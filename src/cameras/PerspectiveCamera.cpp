#include "PerspectiveCamera.h"

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
                                     double fovy,
                                     double near,
                                     double far) :
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

    double aspect = static_cast<double>(m_width) / static_cast<double>(m_height);
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
    auto numThreads = std::thread::hardware_concurrency();
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
                    Color3d pixelColor(0.0);

                    for(int k=0; k < samplesPerPixel; ++k)
                    {
                        auto pixel = glm::dvec2(i + raytracer::randomDouble() - 0.5, j + raytracer::randomDouble() - 0.5);
                        std::unique_ptr<Ray> ray(this->generateThinLensRay(pixel));
                        pixelColor += this->rayColor(ray.get(), m_maxDepth, world);
                    }

                    pixelColor *= 1.0 / samplesPerPixel;
                    pixelColor = gammaCorrect(pixelColor);

                    image[(j * m_width + i) * 3 + 0] = static_cast<uint8_t>(255.0 * clamp(pixelColor.r, 0.0, 1.0));
                    image[(j * m_width + i) * 3 + 1] = static_cast<uint8_t>(255.0 * clamp(pixelColor.g, 0.0, 1.0));
                    image[(j * m_width + i) * 3 + 2] = static_cast<uint8_t>(255.0 * clamp(pixelColor.b, 0.0, 1.0));
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
void PerspectiveCamera::zoom(const double factor)
{
    m_zoomFactor = factor;
    this->setViewAngle(m_fovy / factor);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setViewAngle(const double angle)
{
    if(m_fovy != angle)
    {
        double min = 0.00000001;
        double max = 179.0;

        m_fovy = (angle < min ? min : (angle > max ? max : angle));
        double aspect = static_cast<double>(m_width) / static_cast<double>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setClippingRange(const double near, const double far)
{
    if(m_near != near || m_far != far)
    {
        m_near = near;
        m_far = far;
        double aspect = static_cast<double>(m_width) / static_cast<double>(m_height);

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
        double aspect = static_cast<double>(m_width) / static_cast<double>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }

}

//----------------------------------------------------------------------------------
// Ray *
// PerspectiveCamera::generateThinLensRay(const glm::dvec2 &pixel)
// {
//     Ray *pinholeRay = this->generateRay(pixel);
//     const double aperatureRadius = this->getAperatureRadius();

//     if(aperatureRadius <= 0.0)
//     {
//         return pinholeRay;
//     }

//     auto u = this->getHorizontalAxis();
//     auto v = this->getVerticalAxis();

//     auto defocusDiskU = u * aperatureRadius;
//     auto defocusDiskV = v * aperatureRadius;

//     glm::dvec2 lensOffset = glm::dvec2(randomInUnitDisk());

//     auto origin = this->getPosition() + (lensOffset[0] * defocusDiskU) + (lensOffset[1] * defocusDiskV);
//     glm::dvec3 direction = glm::normalize(pinholeRay->direction() - origin);

//     // Clean-up
//     delete pinholeRay;

//     Ray *lensRay = new Ray(origin, direction);
//     return lensRay;
// }

//----------------------------------------------------------------------------------
Ray *
PerspectiveCamera::generateThinLensRay(const glm::dvec2 &pixel)
{
    Ray *pinholeRay = this->generateRay(pixel);
    const double aperatureRadius = this->getAperatureRadius();

    if(aperatureRadius <= 0.0)
    {
        return pinholeRay;
    }

    glm::dvec2 lensOffset = glm::dvec2(randomInUnitDisk());

    const double focalDistance = 8; // glm::distance(this->getPosition(), this->getFocalPoint());
    const double fstop = focalDistance / (aperatureRadius * 2.0);

    double theta = lensOffset.x * aperatureRadius * 2.0 * M_PI;
    double radius = lensOffset.y;

    auto u = cos(theta) * sqrt(radius);
    auto v = sin(theta) * sqrt(radius);

    glm::dvec3 focusPoint = pinholeRay->direction() * (focalDistance / glm::dot(pinholeRay->direction(), this->getForwardAxis()));
    const double circleOfConfusionRadius = focalDistance / (2.0 * fstop);

    // glm::dvec3 origin = this->getPosition() + (lensOffsetWorld * circleOfConfusion);
    glm::dvec3 origin = this->getPosition() + (u * circleOfConfusionRadius * this->getHorizontalAxis()) + (v * circleOfConfusionRadius * this->getVerticalAxis());
    glm::dvec3 direction = glm::normalize(focusPoint - origin);

    // Clean-up
    delete pinholeRay;

    Ray *lensRay = new Ray(origin, direction, randomDouble());
    return lensRay;
}

//----------------------------------------------------------------------------------
Ray *
PerspectiveCamera::generateRay(const glm::dvec2 &pixel)
{
    const double scale = tan(glm::radians(m_fovy * 0.5));

    // Raster Space -> Normalized Device Coordinate Space
    // TODO: look at NVIDIA's Raytracing Gems book for a better way to do this
    double pxNDC = pixel.x / static_cast<double>(m_width);
    double pyNDC = pixel.y / static_cast<double>(m_height);

    // NDC Space -> Screen Space
    double pxScreen = 2 * pxNDC - 1;
    double pyScreen = 1 - 2 * pyNDC;

    // Screen Space -> Camera Space
    double aspect = static_cast<double>(m_width) / static_cast<double>(m_height);
    double pxCamera = pxScreen * aspect * scale;
    double pyCamera = pyScreen * scale;

    // Camera Space -> World Space
    glm::mat4 cameraToWorldTransform = this->getCameraToWorldMatrix();

    auto rayOrigin = this->getPosition();
    glm::dvec3 rayOriginWorld = glm::mat3(cameraToWorldTransform) * rayOrigin;

    auto focalPoint = this->getFocalPoint();
    glm::dvec3 rayPointWorld = glm::mat3(cameraToWorldTransform) * glm::dvec3(pxCamera, pyCamera, focalPoint.z);

    Ray *ray = new Ray(rayOriginWorld, glm::normalize(rayPointWorld - rayOriginWorld));
    return ray;
}

// Ray *
// PerspectiveCamera::generateRay(const glm::dvec2 &pixel)
// {
//     glm::dvec2 p = (((pixel + glm::dvec2(0.5f)) / glm::dvec2(m_width, m_height)) * 2.0f) - glm::dvec2(1.0f);
//     auto view = this->getViewMatrix();
//     double aspect = static_cast<double>(m_width) / static_cast<double>(m_height);
//     const double scale = tan(glm::radians(m_fovy * 0.5f));

//     Ray *ray = new Ray();
//     ray->m_origin = glm::dvec3(view[3]);
//     ray->m_direction = glm::normalize((p.x * glm::dvec3(view[0]) * aspect * scale) - (p.y * glm::dvec3(view[1]) * scale) + glm::dvec3(view[2]));

//     return ray;
// }

//----------------------------------------------------------------------------------
void
PerspectiveCamera::copy(const ProjectionCamera * const camera)
{
    if(camera != nullptr)
    {
        // Projection Camera
        this->zoom(camera->getZoomFactor());

        glm::dvec2 clippingRange = camera->getClippingRange();
        this->setClippingRange(clippingRange[0], clippingRange[1]);

        glm::dvec2 screenSize = camera->getScreenSize();
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
Color3d PerspectiveCamera::rayColor(Ray * const ray, int depth, const BVH &world)
{
    if(depth <= 0)
    {
        return glm::dvec3(0.0);
    }

    HitRecord record;

    if(world.hit(*ray, record))
    {
        Ray scattered;
        glm::dvec3 attenuation(1.0);

        if(record.material && record.material->scatter(*ray, record, attenuation, scattered))
        {
            return attenuation * rayColor(&scattered, depth-1, world);
            // return gammaCorrect(attenuation * rayColor(&scattered, depth-1, world));
        }

        return glm::dvec3(0.0);
    }

    // auto unitDirection = glm::normalize(ray->direction());
    // double a = 0.5 * (unitDirection.y + 1.0);
    // // return gammaCorrect((1.0f - a) * glm::dvec3(1.0f, 1.0f, 1.0f) + a * glm::dvec3(0.5f, 0.7f, 1.0f));
    // return (1.0 - a) * glm::dvec3(1.0, 1.0, 1.0) + a * glm::dvec3(0.5, 0.7, 1.0);
    return glm::dvec3(0.678, 0.847, 0.902); // sky color
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