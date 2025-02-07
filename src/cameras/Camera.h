#ifndef INCLUDED_CAMERA_H
#define INCLUDED_CAMERA_H

#include <glm/glm.hpp>

namespace raytracer
{
/// @class Camera
/// @brief virtual camera for 3D rendering
///
/// Base camera class that all camera implementations must inherit.
class Camera
{
public:
    Camera();
    virtual ~Camera() = default;

    /// @brief reset camera settings to default values
    virtual void reset();

    //@{
    // Camera Rotation

    /// @brief A rotation of the camera about its forward axis.
    /// @param angle rotation angle in degrees
    void roll(const double angle);

    /// @brief Rotate the camera about the horizontal axis.
    /// @param angle rotation angle in degrees
    void tilt(const double angle);

    /// @brief Rotate the camera about its vertical axis.
    /// @param angle rotation angle in degrees
    void pan(const double angle);
    //@}

    //@{
    // Camera Translation

    /// @brief Translation of the camera along its forward axis
    /// @param value the amount to move the camera
    void dolly(const double value);

    /// @brief Translation of the camera along its vertical axis.
    /// @param value the amount to move the camera
    void boom(const double value);
    //@}

    //@{
    /// Get the normalized forward, horizontal, and vertical axes.
    glm::dvec3 getForwardAxis();
    glm::dvec3 getHorizontalAxis();
    glm::dvec3 getVerticalAxis();
    //@}

    //@{
    /// @brief Set/Get the position of the camera in world coordinates.
    /// @param position position of the camera
    void setPosition(const glm::dvec3 &position);
    glm::dvec3 getPosition() const { return m_position; }
    //@}

    //@{
    /// @brief Set/Get the focal point of the camera in world coordinates. The default
    ///        focal point is the origin.
    /// @param focalPoint the focal point of the camera
    void setFocalPoint(const glm::dvec3 &focalPoint);
    glm::dvec3 getFocalPoint() const { return m_focalPoint; }
    //@}

    //@{
    /// Set/get the camera view up direction. The view up vector will be
    /// normalized.
    void setViewUp(const glm::dvec3 &up);
    glm::dvec3 getViewUp() const { return m_viewUp; }
    //@}

    //@{
    /// @brief Set/Get the aperature radius of the camera.
    /// @param radius the aperature radius
    void setAperatureRadius(const double radius) { m_aperatureRadius = radius; }
    double getAperatureRadius() const { return m_aperatureRadius; }
    //@}

    /// @brief The viewing transformation for moving objects from world space to view space.
    /// @return world to view transformation matrix
    glm::dmat4 getViewMatrix();

    //@{
    /// Set/get the camera to world matrix for moving the camera to world space.
    glm::dmat4 getCameraToWorldMatrix() const { return m_modelMatrix; }
    void setCameraToWorldMatrix(const glm::dmat4 &matrix);
    //@}

    /// @brief Generate a Halton sequence of points in the range [0,1).
    /// @param N the number of points to generate
    /// @param b the base of the sequence
    /// @param out the output array of points
    static void generateHaltonSequence(const int N, const int b, double *out);

private:
    glm::dvec3 m_position;
    glm::dvec3 m_focalPoint;
    glm::dvec3 m_viewUp;

    double     m_aperatureRadius;
    glm::dmat4 m_modelMatrix;
};
} // namespace raytracer

#endif