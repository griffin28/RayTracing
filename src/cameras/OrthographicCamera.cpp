#include "OrthographicCamera.h"

#include <glm/ext/matrix_clip_space.hpp> // glm::ortho

namespace raytracer
{
//----------------------------------------------------------------------------------
OrthographicCamera::OrthographicCamera(int width,
                                       int height,
                                       float fovDistance,
                                       float near,
                                       float far) :
    m_width(width),
    m_height(height),
    m_scale(1.0),
    m_zoomFactor(1.0),
    m_fovDistance(fovDistance),
    m_near(near),
    m_far(far)
{
    float widthd = static_cast<float>(m_width);
    float heightd = static_cast<float>(m_height);
    float aspect =  widthd / heightd;

    float w = widthd * aspect * m_scale * 0.5;
    float h = m_scale * heightd * 0.5;

    float xmin = -1.0 * w;
    float xmax = w;
    float ymin = -1.0 * h;
    float ymax = h;

    m_orthographicMatrix = glm::ortho(xmin, xmax, ymin, ymax, near, far);
}

//----------------------------------------------------------------------------------
void OrthographicCamera::reset()
{
    // TODO: save original values to reset to previous values
    this->Camera::reset();

    m_scale = 1.0f;
    m_fovDistance = 100.0f;
    m_near = 0.1f;
    m_far = 1000.0f;
}

//----------------------------------------------------------------------------------
void OrthographicCamera::setClippingRange(const float near, const float far)
{
    m_near = near;
    m_far = far;

    float widthf = static_cast<float>(m_width);
    float heightf = static_cast<float>(m_height);
    float aspect =  widthf / heightf;

    float w = widthf * aspect * m_scale * 0.5f;
    float h = m_scale * heightf * 0.5f;

    float xmin = w * -1.0f;
    float xmax = w;
    float ymin = h * -1.0f;
    float ymax = h;

    m_orthographicMatrix = glm::ortho(xmin, xmax, ymin, ymax, m_near, m_far);
}

//----------------------------------------------------------------------------------
void OrthographicCamera::setScreenSize(const int width, const int height)
{
    m_width = width;
    m_height = height;

    float widthf = static_cast<float>(m_width);
    float heightf = static_cast<float>(m_height);
    float aspect =  widthf / heightf;

    float w = widthf * aspect * m_scale * 0.5f;
    float h = m_scale * heightf * 0.5f;

    float xmin = w * -1.0f;
    float xmax = w;
    float ymin = h * -1.0f;
    float ymax = h;

    m_orthographicMatrix = glm::ortho(xmin, xmax, ymin, ymax, m_near, m_far);
}

//----------------------------------------------------------------------------------
void OrthographicCamera::zoom(const float factor)
{
    m_zoomFactor = factor;
    m_scale /= factor;

    float widthf = static_cast<float>(m_width);
    float heightf = static_cast<float>(m_height);
    float aspect =  widthf / heightf;

    float w = widthf * aspect * m_scale * 0.5;
    float h = m_scale * heightf * 0.5;

    float xmin = w * -1.0;
    float xmax = w;
    float ymin = h * -1.0;
    float ymax = h;

    m_orthographicMatrix = glm::ortho(xmin, xmax, ymin, ymax, m_near, m_far);
}

//----------------------------------------------------------------------------------
Ray *
OrthographicCamera::generateRay(const glm::vec2 &pixel)
{
    auto p = pixel * m_fovDistance;
    auto origin = glm::vec3(p, 0.0f);

    Ray *ray = new Ray(origin,
                       glm::vec3(0.0f, 0.0f, -1.0f),
                       std::abs(m_near),
                       std::abs(m_far));

    return ray;
}

//----------------------------------------------------------------------------------
void
OrthographicCamera::copy(const ProjectionCamera * const camera)
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
} // namespace raytracer