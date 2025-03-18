#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>	 // translate, rotate, scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

namespace raytracer
{
//----------------------------------------------------------------------------------
Camera::Camera()
    : m_position(glm::dvec3(0.0, 0.0, 0.0))
    , m_focalPoint(glm::dvec3(0.0, 0.0, -1.0))
    , m_viewUp(glm::dvec3(0.0, 1.0, 0.0))
    , m_background(Color3d(0.0, 0.0, 0.0))
    , m_aperatureRadius(0.0)
    , m_modelMatrix(glm::dmat4(1.0))
{}

//----------------------------------------------------------------------------------
void Camera::reset()
{
    m_position = glm::dvec3(0.0, 0.0, 0.0);
    m_focalPoint = glm::dvec3(0.0, 0.0, -1.0);
    m_viewUp = glm::dvec3(0.0, 1.0, 0.0);
    m_background = Color3d(0.0, 0.0, 0.0);
    m_aperatureRadius = 0.0;
    m_modelMatrix = glm::dmat4(1.0);
}

//----------------------------------------------------------------------------------
void Camera::roll(const double angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                this->getForwardAxis());
}

//----------------------------------------------------------------------------------
void Camera::tilt(const double angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                this->getHorizontalAxis());
}

//----------------------------------------------------------------------------------
void Camera::pan(const double angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                this->getVerticalAxis());
}

//----------------------------------------------------------------------------------
void Camera::dolly(const double value)
{
    double distance = glm::distance(m_focalPoint, m_position);
    double d = distance + value;
    d = (d <= 0.0) ? 0.1 : d;

    auto forwardAxis = this->getForwardAxis();
    auto delta = forwardAxis * d;

    m_modelMatrix = glm::translate(m_modelMatrix, -delta);
}

//----------------------------------------------------------------------------------
void Camera::boom(const double value)
{
    auto verticalAxis = this->getVerticalAxis();
    auto delta = verticalAxis * value;

    m_modelMatrix = glm::translate(m_modelMatrix, -delta);
}

//----------------------------------------------------------------------------------
void Camera::setPosition(const glm::dvec3 &position)
{
    m_position = position;
}

//----------------------------------------------------------------------------------
glm::dvec3 Camera::getWorldPosition() const
{
    auto worldPos = m_modelMatrix * glm::dvec4(m_position, 1.0);
    return glm::dvec3(worldPos);
}

//----------------------------------------------------------------------------------
void Camera::setFocalPoint(const glm::dvec3 &focalPoint)
{
    m_focalPoint = focalPoint;
}

//----------------------------------------------------------------------------------
void Camera::setViewUp(const glm::dvec3 &up)
{
    m_viewUp = glm::normalize(up);
}

//----------------------------------------------------------------------------------
glm::dvec3 Camera::getForwardAxis()
{
    auto worldPos = this->getWorldPosition();
    return glm::normalize(m_focalPoint - worldPos);
}

//----------------------------------------------------------------------------------
glm::dvec3 Camera::getHorizontalAxis()
{
    auto forwardAxis = this->getForwardAxis();
    return glm::cross(forwardAxis, m_viewUp);
}

//----------------------------------------------------------------------------------
glm::dvec3 Camera::getVerticalAxis()
{
    auto forwardAxis = this->getForwardAxis();
    auto horizontalAxis = this->getHorizontalAxis();
    return glm::cross(horizontalAxis, forwardAxis);
}

//----------------------------------------------------------------------------------
void Camera::setCameraToWorldMatrix(const glm::dmat4 &matrix)
{
    m_modelMatrix = matrix;
}

//----------------------------------------------------------------------------------
glm::dmat4 Camera::getViewMatrix()
{
    auto worldPos = this->getWorldPosition();
    return glm::lookAt(worldPos, m_focalPoint, m_viewUp);
}

//----------------------------------------------------------------------------------
void Camera::generateHaltonSequence(const int N, const int b, double *out)
{
    for (int i = 0; i < N; ++i)
    {
        int index = i;
        double f = 1.0;
        double r = 0.0;

        while (index > 0)
        {
            f = f / b;
            r = r + f * (index % b);
            index = index / b;
        }

        out[i] = r;
    }
}

// void Camera::generateHaltonSequence(const int N, const int b, double *out)
// {
//     int n = 0;
//     int d = 1;

//     for (int i = 0; i < N; ++i)
//     {
//         int x = d - n;

//         if(x == 1)
//         {
//             n = 1;
//             d *= b;
//         }
//         else
//         {
//             auto y = d / b;

//             while(x <= y)
//             {
//                 y /= b;
//             }

//             n = (b + 1) * y - x;
//         }

//         out[i] = static_cast<double>(n) / static_cast<double>(d);
//     }
// }
} // namespace raytracer