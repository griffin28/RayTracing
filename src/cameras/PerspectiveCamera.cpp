#include "PerspectiveCamera.h"
#include "HittablePdf.h"
#include "CosinePdf.h"
#include "MixturePdf.h"

#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

#include <thread>
#include <functional> // std::bind

namespace raytracer
{
//----------------------------------------------------------------------------------
PerspectiveCamera::PerspectiveCamera()
    : PerspectiveCamera(800, 600)
{
}

//----------------------------------------------------------------------------------
PerspectiveCamera::PerspectiveCamera(int width,
                                     int height,
                                     int maxDepth,
                                     float fovy,
                                     float near,
                                     float far) :
    m_width(width),
    m_height(height),
    m_maxDepth(maxDepth),
    m_zoomFactor(1.0),
    m_fovy(fovy),
    m_near(near),
    m_far(far)
{
    if(m_width <= 0 || m_height <= 0)
    {
        throw std::invalid_argument("Invalid screen size");
    }

    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::reset()
{
    // TODO: save original values to reset to previous values
    this->Camera::reset();

    m_fovy = 45.0;
    m_near = 0.1;
    m_far = 1000.0;
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::render(const BVH &world, const int samplesPerPixel, std::ostream &out)
{
    std::unique_ptr<uint8_t[]> image(new uint8_t[m_width * m_height * 3]);
    auto numThreads = std::thread::hardware_concurrency() * 4;
    numThreads = numThreads > m_height ? m_height : numThreads;
    std::clog << "Using " << numThreads << " threads\n";
    std::vector<std::thread> threads(numThreads);

    for(int t=0; t<numThreads; t++)
    {
        // std::bind is used to pass the parameters to the lambda function
        threads[t] = std::thread(std::bind([&](int start, int end, int t)
        {
            for(int j=start; j < end; ++j)
            {
                if(t == static_cast<int>((numThreads / 2)))
                {
                    std::clog << "\rScanlines remaining: " << end - j << ' ' << std::flush;
                }

                for(int i=0; i < m_width; ++i)
                {
                    Color3f pixelColor(0.0f);

                    for(int k=0; k < samplesPerPixel; ++k)
                    {
                        auto pixel = glm::vec2(i + RaytracingUtility::randomDouble() - 0.5f, j + RaytracingUtility::randomDouble() - 0.5f);
                        std::unique_ptr<Ray> ray(this->generateThinLensRay(pixel));
                        pixelColor += this->rayColor(ray.get(), m_maxDepth, world);
                    }

                    pixelColor *= 1.0f / samplesPerPixel;
                    pixelColor = RaytracingUtility::gammaCorrect(pixelColor);

                    image[(j * m_width + i) * 3 + 0] = static_cast<uint8_t>(255.0f * RaytracingUtility::clamp(pixelColor.r, 0.0f, 1.0f));
                    image[(j * m_width + i) * 3 + 1] = static_cast<uint8_t>(255.0f * RaytracingUtility::clamp(pixelColor.g, 0.0f, 1.0f));
                    image[(j * m_width + i) * 3 + 2] = static_cast<uint8_t>(255.0f * RaytracingUtility::clamp(pixelColor.b, 0.0f, 1.0f));
                }
            }
        }, t * m_height / numThreads, (t+1) == numThreads ? m_height : (t+1) * m_height / numThreads, t));
    }

    for(auto &thread : threads)
    {
        thread.join();
    }

    this->writePPMImage(image.get(), m_width, m_height, out);
    std::clog << "\nDone.\n";
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::zoom(const float factor)
{
    m_zoomFactor = factor;
    this->setViewAngle(m_fovy / factor);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setViewAngle(const float angle)
{
    if(m_fovy != angle)
    {
        float min = 0.00000001f;
        float max = 179.0f;

        m_fovy = (angle < min ? min : (angle > max ? max : angle));
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setClippingRange(const float near, const float far)
{
    if(m_near != near || m_far != far)
    {
        m_near = near;
        m_far = far;
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setScreenSize(const int width, const int height)
{
    if(m_width != width || m_height != height)
    {
        m_width = width;
        m_height = height;
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }

}

//----------------------------------------------------------------------------------
// Ray *
// PerspectiveCamera::generateThinLensRay(const glm::vec2 &pixel)
// {
//     Ray *pinholeRay = this->generateRay(pixel);
//     const float aperatureRadius = this->getAperatureRadius();

//     if(aperatureRadius <= 0.0)
//     {
//         return pinholeRay;
//     }

//     auto u = this->getHorizontalAxis();
//     auto v = this->getVerticalAxis();

//     auto defocusDiskU = u * aperatureRadius;
//     auto defocusDiskV = v * aperatureRadius;

//     glm::vec2 lensOffset = glm::vec2(randomInUnitDisk());

//     auto origin = this->getPosition() + (lensOffset[0] * defocusDiskU) + (lensOffset[1] * defocusDiskV);
//     glm::vec3 direction = glm::normalize(pinholeRay->direction() - origin);

//     // Clean-up
//     delete pinholeRay;

//     Ray *lensRay = new Ray(origin, direction);
//     return lensRay;
// }

//----------------------------------------------------------------------------------
Ray *
PerspectiveCamera::generateThinLensRay(const glm::vec2 &pixel)
{
    Ray *pinholeRay = this->generateRay(pixel);
    const float aperatureRadius = this->getAperatureRadius();

    if(aperatureRadius <= 0.0)
    {
        return pinholeRay;
    }

    glm::vec2 lensOffset = glm::vec2(RaytracingUtility::randomInUnitDisk());

    const float focalDistance = 8; // glm::distance(this->getPosition(), this->getFocalPoint());
    const float fstop = focalDistance / (aperatureRadius * 2.0f);

    float theta = lensOffset.x * aperatureRadius * 2.0f * M_PI;
    float radius = lensOffset.y;

    float u = static_cast<float>(cos(theta) * sqrt(radius));
    float v = static_cast<float>(sin(theta) * sqrt(radius));

    glm::vec3 focusPoint = pinholeRay->direction() * (focalDistance / glm::dot(pinholeRay->direction(), this->getForwardAxis()));
    const float circleOfConfusionRadius = focalDistance / (2.0f * fstop);

    // glm::vec3 origin = this->getPosition() + (lensOffsetWorld * circleOfConfusion);
    glm::vec3 origin = this->getPosition() + (u * circleOfConfusionRadius * this->getHorizontalAxis()) + (v * circleOfConfusionRadius * this->getVerticalAxis());
    glm::vec3 direction = glm::normalize(focusPoint - origin);

    // Clean-up
    delete pinholeRay;

    Ray *lensRay = new Ray(origin, direction, RaytracingUtility::randomDouble());
    return lensRay;
}

//----------------------------------------------------------------------------------
// TODO: Look at using the perspective matrix to generate the ray
Ray *
PerspectiveCamera::generateRay(const glm::vec2 &pixel)
{
    const float scale = tan(glm::radians(m_fovy * 0.5f));

    // Raster Space -> Normalized Device Coordinate Space
    float pxNDC = pixel.x / static_cast<float>(m_width);
    float pyNDC = pixel.y / static_cast<float>(m_height);

    // NDC Space -> Screen Space
    float pxScreen = 2 * pxNDC - 1;
    float pyScreen = 1 - 2 * pyNDC;

    // Screen Space -> Camera Space
    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    float pxCamera = pxScreen * aspect * scale;
    float pyCamera = pyScreen * scale;

    // Camera Space -> World Space
    glm::mat4 cameraToWorldTransform = this->getCameraToWorldMatrix();

    glm::vec3 rayOrigin = this->getPosition();
    glm::vec3 rayOriginWorld = glm::vec3(cameraToWorldTransform * glm::vec4(rayOrigin, 1.0f));

    // Camera coordinate frame
    auto u = this->getHorizontalAxis();
    auto v = glm::vec3(-1) * this->getVerticalAxis();
    auto w = this->getForwardAxis();

    glm::vec3 direction = (pxCamera * u) - (pyCamera * v) + w;
    auto directionWorld = glm::normalize(glm::vec3(cameraToWorldTransform * glm::vec4(direction, 1.0f)));

    Ray *ray = new Ray(rayOriginWorld, directionWorld);
    return ray;
}

//----------------------------------------------------------------------------------
void
PerspectiveCamera::copy(const ProjectionCamera * const camera)
{
    if(camera != nullptr)
    {
        // Projection Camera
        this->zoom(camera->getZoomFactor());

        glm::vec2 clippingRange = camera->getClippingRange();
        this->setClippingRange(clippingRange[0], clippingRange[1]);

        glm::vec2 screenSize = camera->getScreenSize();
        this->setScreenSize(screenSize.x, screenSize.y);

        // Camera
        this->setPosition(camera->getPosition());
        this->setFocalPoint(camera->getFocalPoint());
        this->setViewUp(camera->getViewUp());

        this->setCameraToWorldMatrix(camera->getCameraToWorldMatrix());
    }
    else
    {
        this->reset();
    }
}

//----------------------------------------------------------------------------------
Color3f PerspectiveCamera::rayColor(Ray * const ray, int depth, const BVH &world)
{
    if(depth <= 0)
    {
        return Color3f(0.0f);
    }

    HitRecord record;

    if(world.hit(*ray, record))
    {
        Ray scattered;
        glm::vec3 attenuation(1.f);
        float pdfValue = 1.0f;
        Color3f emitted = record.material->emitted(record);

        if(!record.material->scatter(*ray, record, attenuation, scattered, pdfValue))
        {
            return emitted;
        }

        std::vector<std::shared_ptr<Pdf>> pdfs;
        // pdfs.push_back(std::make_shared<CosinePdf>(record.normal));
        auto lightSources = world.getLightSources();

        for(const auto &light : lightSources)
        {
            pdfs.push_back(std::make_shared<HittablePdf>(light, record.point));
        }

        if(!pdfs.empty())
        {
            MixturePdf mixturePdf(pdfs);
            scattered = Ray(record.point, mixturePdf.generate());
            pdfValue = mixturePdf.value(scattered.direction());
        }

        // Importance sampling using the material's scattering PDF
        float scatteringPDF = record.material->scatteringPDF(*ray, record, scattered);        
        auto colorFromScatter = (attenuation * scatteringPDF * rayColor(&scattered, depth-1, world))  / pdfValue;

        return emitted + colorFromScatter;
    }

    return this->getBackgroundColor();
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::writePPMImage(uint8_t *image, const int width, const int height, std::ostream &out)
{
    out << "P3\n" << width << ' ' << height << "\n255\n";

    for(int j=0; j < height; j++)
    {
        for(int i=0; i < width; ++i)
        {
            out << static_cast<int>(image[(j * width + i) * 3 + 0]) << ' '
                << static_cast<int>(image[(j * width + i) * 3 + 1]) << ' '
                << static_cast<int>(image[(j * width + i) * 3 + 2]) << '\n';
        }
    }
}
} // namespace raytracer