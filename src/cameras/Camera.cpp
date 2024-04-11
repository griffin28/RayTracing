#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>	 // translate, rotate, scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

namespace raytracer
{
//----------------------------------------------------------------------------------
Camera::Camera() : m_position(glm::dvec3(0.f, 0.f, 0.f)),
                   m_focalPoint(glm::dvec3(0.f, 0.f, -1.f)),
                   m_viewUp(glm::dvec3(0.0f, 1.0f, 0.0f)),
                   m_aperatureRadius(0.0f),
                   m_modelMatrix(glm::dmat4(1.0f))
{
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::reset()
{
    m_position = glm::dvec3(0.0f, 0.0f, 0.0f);
    m_focalPoint = glm::dvec3(0.f, 0.f, -1.f);
    m_viewUp = glm::dvec3(0.0f, 1.0f, 0.0f);
    m_modelMatrix = glm::dmat4(1.0f);

    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::roll(const double angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                this->getForwardAxis());
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::tilt(const double angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                this->getHorizontalAxis());
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::pan(const double angle)
{
    m_modelMatrix = glm::rotate(m_modelMatrix,
                                glm::radians(angle),
                                this->getVerticalAxis());
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::dolly(const double value)
{
    double distance = glm::distance(m_focalPoint, m_position);
    double d = distance + value;
    d = (d <= 0.0f) ? 0.1f : d;

    auto forwardAxis = this->getForwardAxis();
    auto delta = forwardAxis * d;

    this->setPosition(m_focalPoint - delta);
}

//----------------------------------------------------------------------------------
void Camera::boom(const double value)
{
    auto verticalAxis = this->getVerticalAxis();
    auto delta = verticalAxis * value;

    this->setPosition(m_position - delta);
}

//----------------------------------------------------------------------------------
void Camera::setPosition(const glm::dvec3 &position)
{
    m_position = position;
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::setFocalPoint(const glm::dvec3 &focalPoint)
{
    m_focalPoint = focalPoint;
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::setViewUp(const glm::dvec3 &up)
{
    m_viewUp = glm::normalize(up);
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
glm::dvec3 Camera::getForwardAxis()
{
    return glm::normalize(m_focalPoint - m_position);
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
    auto horizontalAxis = glm::cross(m_viewUp, forwardAxis);
    return glm::cross(horizontalAxis, forwardAxis);
}

//----------------------------------------------------------------------------------
void Camera::setCameraToWorldMatrix(const glm::dmat4 &matrix)
{
    m_modelMatrix = matrix;
    this->updateViewMatrix();
}

//----------------------------------------------------------------------------------
void Camera::updateViewMatrix()
{
    auto position = m_modelMatrix * glm::vec4(m_position, 1.0f);
    auto viewUp = m_modelMatrix * glm::vec4(m_viewUp, 1.0f);

    auto pos = glm::dvec3(position[0]/position[3],
                         position[1]/position[3],
                         position[2]/position[3]);

    auto up = glm::normalize(glm::dvec3(viewUp[0]/viewUp[3],
                                       viewUp[1]/viewUp[3],
                                       viewUp[2]/viewUp[3]));

    m_viewMatrix = glm::lookAt(pos, m_focalPoint, up);
}

//----------------------------------------------------------------------------------
void Camera::generateHaltonSequence(const int N, const int b, double *out)
{
    for (int i = 0; i < N; ++i)
    {
        int index = i;
        double f = 1.0f;
        double r = 0.0f;

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