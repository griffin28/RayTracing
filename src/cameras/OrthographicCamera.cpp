#include "OrthographicCamera.h"

#include <glm/ext/matrix_clip_space.hpp> // glm::ortho

namespace raytracer
{
//----------------------------------------------------------------------------------
OrthographicCamera::OrthographicCamera(int width,
                                       int height,
                                       double fovDistance,
                                       double near,
                                       double far) :
    m_width(width),
    m_height(height),
    m_scale(1.0),
    m_zoomFactor(1.0),
    m_fovDistance(fovDistance),
    m_near(near),
    m_far(far)
{
    double widthd = static_cast<double>(m_width);
    double heightd = static_cast<double>(m_height);
    double aspect =  widthd / heightd;

    double w = widthd * aspect * m_scale * 0.5;
    double h = m_scale * heightd * 0.5;

    double xmin = -1.0 * w;
    double xmax = w;
    double ymin = -1.0 * h;
    double ymax = h;

    m_orthographicMatrix = glm::ortho(xmin, xmax, ymin, ymax, near, far);
}

//----------------------------------------------------------------------------------
void OrthographicCamera::reset()
{
    // TODO: save original values to reset to previous values
    this->Camera::reset();

    m_scale = 1.0;
    m_fovDistance = 100.0;
    m_near = 0.1;
    m_far = 1000.0;
}

//----------------------------------------------------------------------------------
void OrthographicCamera::setClippingRange(const double near, const double far)
{
    m_near = near;
    m_far = far;

    double widthf = static_cast<double>(m_width);
    double heightf = static_cast<double>(m_height);
    double aspect =  widthf / heightf;

    double w = widthf * aspect * m_scale * 0.5;
    double h = m_scale * heightf * 0.5;

    double xmin = w * -1.0;
    double xmax = w;
    double ymin = h * -1.0;
    double ymax = h;

    m_orthographicMatrix = glm::ortho(xmin, xmax, ymin, ymax, m_near, m_far);
}

//----------------------------------------------------------------------------------
void OrthographicCamera::setScreenSize(const int width, const int height)
{
    m_width = width;
    m_height = height;

    double widthf = static_cast<double>(m_width);
    double heightf = static_cast<double>(m_height);
    double aspect =  widthf / heightf;

    double w = widthf * aspect * m_scale * 0.5;
    double h = m_scale * heightf * 0.5;

    double xmin = w * -1.0;
    double xmax = w;
    double ymin = h * -1.0;
    double ymax = h;

    m_orthographicMatrix = glm::ortho(xmin, xmax, ymin, ymax, m_near, m_far);
}

//----------------------------------------------------------------------------------
void OrthographicCamera::zoom(const double factor)
{
    m_zoomFactor = factor;
    m_scale /= factor;

    double widthf = static_cast<double>(m_width);
    double heightf = static_cast<double>(m_height);
    double aspect =  widthf / heightf;

    double w = widthf * aspect * m_scale * 0.5;
    double h = m_scale * heightf * 0.5;

    double xmin = w * -1.0;
    double xmax = w;
    double ymin = h * -1.0;
    double ymax = h;

    m_orthographicMatrix = glm::ortho(xmin, xmax, ymin, ymax, m_near, m_far);
}

//----------------------------------------------------------------------------------
Ray *
OrthographicCamera::generateRay(const glm::dvec2 &pixel)
{
    auto p = pixel * m_fovDistance;
    auto origin = glm::dvec3(p, 0.0);

    Ray *ray = new Ray(origin,
                       glm::dvec3(0.0, 0.0, -1.0),
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
} // namespace raytracer