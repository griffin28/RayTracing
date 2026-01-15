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
    : PerspectiveCamera(800, 600, 10)
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

    const int sqrtspp = static_cast<int>(std::sqrt(samplesPerPixel));
    const float pixelSamplesScale = 1.0f / (sqrtspp * sqrtspp);

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

                    for(int sj = 0; sj < sqrtspp; ++sj)
                    {
                        for(int si = 0; si < sqrtspp; ++si)
                        {
                            // auto pixel = glm::vec2(i + RaytracingUtility::randomDouble() - 0.5f, j + RaytracingUtility::randomDouble() - 0.5f);
                            auto offset = this->sampleSquareStratified(si, sj, samplesPerPixel);
                            auto pixel = glm::vec2(i + offset.x, j + offset.y);
                            pixel += glm::vec2(0.5f, 0.5f); // Center of the pixel
                            std::unique_ptr<Ray> ray(this->generateThinLensRay(pixel));
                            // std::clog << "Generated Ray: " << *ray << "\n";
                            pixelColor += this->rayColor(ray.get(), m_maxDepth, world);
                        }
                    }

                    pixelColor *= pixelSamplesScale;
                    pixelColor = glm::clamp(RaytracingUtility::gammaCorrect(pixelColor), 0.0f, 1.0f);

                    image[(j * m_width + i) * 3 + 0] = static_cast<uint8_t>(255.0f * pixelColor.r);
                    image[(j * m_width + i) * 3 + 1] = static_cast<uint8_t>(255.0f * pixelColor.g);
                    image[(j * m_width + i) * 3 + 2] = static_cast<uint8_t>(255.0f * pixelColor.b);
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
//     const float apertureRadius = this->getApertureRadius();

//     if(apertureRadius <= 0.0f)
//     {
//         return pinholeRay;
//     }

//     auto u = this->getHorizontalAxis();
//     auto v = this->getVerticalAxis();

//     auto defocusDiskU = u * apertureRadius;
//     auto defocusDiskV = v * apertureRadius;

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
    const float apertureRadius = this->getApertureRadius();

    if(apertureRadius <= 0.0f)
    {
        return pinholeRay;
    }

    glm::vec2 lensOffset = glm::vec2(RaytracingUtility::randomInUnitDisk());

    const float focalDistance = glm::distance(this->getWorldPosition(), this->getFocalPoint());
    const float fstop = focalDistance / (apertureRadius * 2.0f);

    float theta = lensOffset.x * apertureRadius * 2.0f * glm::pi<float>();
    float radius = lensOffset.y;

    float u = glm::cos(theta) * glm::sqrt(radius);
    float v = glm::sin(theta) * glm::sqrt(radius);

    glm::vec3 focusPoint = pinholeRay->direction() * (focalDistance / glm::dot(pinholeRay->direction(), this->getForwardAxis()));
    const float circleOfConfusionRadius = focalDistance / (2.0f * fstop);

    // glm::vec3 origin = this->getPosition() + (lensOffsetWorld * circleOfConfusion);
    glm::vec3 origin = this->getPosition() + (u * circleOfConfusionRadius * this->getHorizontalAxis()) + (v * circleOfConfusionRadius * this->getVerticalAxis());
    glm::vec3 direction = glm::normalize(focusPoint - origin);

    // Clean-up
    delete pinholeRay;

    Ray *lensRay = new Ray(origin, direction);
    return lensRay;
}

//----------------------------------------------------------------------------------
Ray *
PerspectiveCamera::generateRay(const glm::vec2 &pixel)
{
    // Raster Space -> Normalized Device Coordinate Space [-1,1]
    float pxN = pixel.x / static_cast<float>(m_width-1);
    float pyN = pixel.y / static_cast<float>(m_height-1);

    float pxNDC = 2 * pxN - 1;
    float pyNDC = 1 - 2 * pyN;

    // NDC Space -> Viewport Plane Coordinates in Camera Space   
    const float verticalHalfSize = tan(glm::radians(m_fovy * 0.5f));
    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    const float horizontalHalfSize = aspect * verticalHalfSize;

    float pxView = pxNDC * horizontalHalfSize * m_near;
    float pyView = pyNDC * verticalHalfSize * m_near;
    float pzView = m_near;

    // Camera coordinate frame
    auto u = this->getHorizontalAxis();
    auto v = this->getVerticalAxis();
    auto w = this->getForwardAxis();

    // Camera Space -> World Space    
    glm::vec3 cameraOriginWorld = this->getWorldPosition();
    glm::vec3 cameraDirectionWorld = glm::normalize((pxView * u) + (pyView * v) + (pzView * w));

    Ray *ray = new Ray(cameraOriginWorld, cameraDirectionWorld);
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
        // std::clog << "Max ray depth reached\n";
        return Color3f(0.0f);
    }

    HitRecord record;

    if(world.hit(*ray, record))
    {        
        Color3f emitted = record.material->emitted(record);

        Color3f attenuation(1.f);
        Ray scattered;
        float pdfValue;

        if(!record.material->scatter(*ray, record, attenuation, scattered, pdfValue))
        {
            return emitted;
        }
        
        // auto lightSources = world.getLightSources();
        // auto lightPos = lightSources[0]->center();  // lightSources[0]->randomPointOnSurface();
        // auto lightDir = lightPos - record.point;
        // scattered = Ray(record.point, glm::normalize(lightDir));

        // if(glm::dot(scattered.direction(), record.normal) < 0.0f) // Light is below the surface
        // {
        //     // std::clog << "Ray:" << *ray << "\n";
        //     // std::clog << "recordPoint: (" << record.point.x << ", " << record.point.y << ", " << record.point.z << ")\n";
        //     // std::clog << "record.normal: (" << record.normal.x << ", " << record.normal.y << ", " << record.normal.z << ")\n";
        //     // std::clog << "lightPos: (" << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << ")\n";
        //     // std::clog << "lightDir: (" << lightDir.x << ", " << lightDir.y << ", " << lightDir.z << ")\n";
            
        //     return emitted;
        // }

        // auto distanceSquared = glm::dot(lightDir, lightDir);
        // auto lightArea = lightSources[0]->getSurfaceArea();
        // auto lightCosine = glm::abs(scattered.direction().y);

        // if(lightCosine < 0.000001f) // Light is edge-on to the surface
        // {
        //     return emitted;
        // }

        auto lightSources = world.getLightSources();
        std::vector<std::shared_ptr<Pdf>> pdfs;
        // CosinePdf cosinePdf(record.normal);
        // pdfs.push_back(std::make_shared<CosinePdf>(record.normal));

        for(const auto &light : lightSources)
        {
            pdfs.push_back(std::make_shared<HittablePdf>(light, record.point));
        }

        if(!pdfs.empty())
        {
            MixturePdf mixturePdf(pdfs);
            scattered = Ray(record.point, glm::normalize(mixturePdf.generate()));
            pdfValue = mixturePdf.value(scattered.direction());
        }
        
        float scatteringPDF = record.material->scatteringPDF(*ray, record, scattered);
        Color3f colorFromScatter = (attenuation * scatteringPDF * rayColor(&scattered, depth-1, world)) / pdfValue;
        
        return emitted + colorFromScatter;
    }

    std::clog << "Ray miss - returning background color\n";
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