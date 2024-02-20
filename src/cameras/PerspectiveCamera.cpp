#include "PerspectiveCamera.h"
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
                                     float fovy,
                                     float near,
                                     float far) :
    m_width(width),
    m_height(height),
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
void PerspectiveCamera::render(const HittableList &world, std::ostream &out)
{
    out << "P3\n" << m_width << ' ' << m_height << "\n255\n";

    for(int j=0; j < m_height; ++j)
    {
        std::clog << "\rScanlines remaining: " << m_height - j << ' ' << std::flush;
        for(int i=0; i < m_width; ++i)
        {
            std::unique_ptr<Ray> ray(this->generateRay(glm::vec2(i, j)));
            auto pixelColor = this->rayColor(ray.get(), world);
            this->writeColor3f(out, pixelColor);
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
    float pxNDC = (pixel.x + 0.5f) / static_cast<float>(m_width);
    float pyNDC = (pixel.y + 0.5f) / static_cast<float>(m_height);

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
PerspectiveCamera::copy(ProjectionCamera * const camera)
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
glm::vec3 PerspectiveCamera::rayColor(Ray * const ray, const HittableList &world)
{
    HitRecord record;
    if(world.hit(*ray, record))
    {
        return 0.5f * glm::vec3(record.normal.x + 1, record.normal.y + 1, record.normal.z + 1);
    }

    float a = 0.5f * (ray->direction().y + 1.0f);
    return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::writeColor3f(std::ostream& out, glm::vec3 pixelColor)
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixelColor.x) << ' '
        << static_cast<int>(255.999 * pixelColor.y) << ' '
        << static_cast<int>(255.999 * pixelColor.z) << '\n';
}
} // namespace raytracer