#include "PerspectiveCamera.h"
#include "Utility.h"
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

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
    m_zoomFactor(1.0f),
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

    m_fovy = 45.0f;
    m_near = 0.1f;
    m_far = 1000.0f;
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::render(const HittableList &world, int samplesPerPixel, std::ostream &out)
{
    out << "P3\n" << m_width << ' ' << m_height << "\n255\n";

    for(int j=0; j < m_height; ++j)
    {
        std::clog << "\rScanlines remaining: " << m_height - j << ' ' << std::flush;
        for(int i=0; i < m_width; ++i)
        {
            glm::vec3 pixelColor(0.0f);
            for(int spp=0; spp < samplesPerPixel; ++spp)
            {
                std::unique_ptr<Ray> ray(this->generateRay(glm::vec2(i, j)));
                pixelColor += this->rayColor(ray.get(), m_maxDepth, world);
            }

            this->writeColor3f(out, pixelColor, samplesPerPixel);
        }
    }

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
Ray *
PerspectiveCamera::generateRay(const glm::vec2 &pixel)
{
    const float scale = tan(glm::radians(m_fovy * 0.5f));

    // Raster Space -> Normalized Device Coordinate Space
    // TODO: look at NVIDIA's Raytracing Gems book for a better way to do this
    float pxNDC = (pixel.x - 0.5f + randomFloat()) / static_cast<float>(m_width);
    float pyNDC = (pixel.y - 0.5f + randomFloat()) / static_cast<float>(m_height);

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
    glm::vec3 rayOriginWorld = glm::mat3(cameraToWorldTransform) * rayOrigin;
    glm::vec3 rayPointWorld = glm::mat3(cameraToWorldTransform) * glm::vec3(pxCamera, pyCamera, rayOrigin.z-1);

    Ray *ray = new Ray(rayOriginWorld, glm::normalize(rayPointWorld - rayOriginWorld));
    return ray;
}

// Ray *
// PerspectiveCamera::generateRay(const glm::vec2 &pixel)
// {
//     glm::vec2 p = (((pixel + glm::vec2(0.5f)) / glm::vec2(m_width, m_height)) * 2.0f) - glm::vec2(1.0f);
//     auto view = this->getViewMatrix();
//     float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
//     const float scale = tan(glm::radians(m_fovy * 0.5f));

//     Ray *ray = new Ray();
//     ray->m_origin = glm::vec3(view[3]);
//     ray->m_direction = glm::normalize((p.x * glm::vec3(view[0]) * aspect * scale) - (p.y * glm::vec3(view[1]) * scale) + glm::vec3(view[2]));

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
glm::vec3 PerspectiveCamera::rayColor(Ray * const ray, int depth, const HittableList &world)
{
    HitRecord record;

    if(depth <= 0)
    {
        return glm::vec3(0.0f);
    }

    if(world.hit(*ray, record))
    {
        Ray scattered;
        glm::vec3 attenuation(1.f);

        if(record.material->scatter(*ray, record, attenuation, scattered))
        {
            // return attenuation * rayColor(&scattered, depth-1, world);
            return gammaCorrect(attenuation * rayColor(&scattered, depth-1, world));
        }

        return glm::vec3(0.0f);
    }

    auto unitDirection = glm::normalize(ray->direction());
    float a = 0.5f * (unitDirection.y + 1.0f);
    return gammaCorrect((1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f));
    // return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::writeColor3f(std::ostream& out, glm::vec3 pixelColor, const int samplesPerPixel)
{
    const float scale = 1.0f / static_cast<float>(samplesPerPixel);
    pixelColor *= scale;

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256.f * clamp(pixelColor.x, 0.000f, 0.999f)) << ' '
        << static_cast<int>(256.f * clamp(pixelColor.y, 0.000f, 0.999f)) << ' '
        << static_cast<int>(256.f * clamp(pixelColor.z, 0.000f, 0.999f)) << '\n';
}
} // namespace raytracer