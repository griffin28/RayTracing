#ifndef INCLUDED_CAMERA_H
#define INCLUDED_CAMERA_H

#include <glm/glm.hpp>
#include <Utility.h>

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
    void roll(const float angle);

    /// @brief Rotate the camera about the horizontal axis.
    /// @param angle rotation angle in degrees
    void tilt(const float angle);

    /// @brief Rotate the camera about its vertical axis.
    /// @param angle rotation angle in degrees
    void pan(const float angle);
    //@}

    //@{
    // Camera Translation

    /// @brief Translation of the camera along its forward axis
    /// @param value the amount to move the camera
    void dolly(const float value);

    /// @brief Translation of the camera along its vertical axis.
    /// @param value the amount to move the camera
    void boom(const float value);
    //@}

    //@{
    /// Get the normalized forward, horizontal, and vertical axes.
    glm::vec3 getForwardAxis();
    glm::vec3 getHorizontalAxis();
    glm::vec3 getVerticalAxis();
    //@}

    //@{
    /// @brief Set/Get the position of the camera in object space.
    /// @param position position of the camera
    void setPosition(const glm::vec3 &position);
    glm::vec3 getPosition() const { return m_position; }
    //@}

    /// @brief Get the position of the camera in world space.
    /// @return the position of the camera in world space
    glm::vec3 getWorldPosition() const;

    //@{
    /// @brief Set/Get the focal point of the camera in world coordinates. The default
    ///        focal point is the origin.
    /// @param focalPoint the focal point of the camera
    void setFocalPoint(const glm::vec3 &focalPoint);
    glm::vec3 getFocalPoint() const { return m_focalPoint; }
    //@}

    //@{
    /// Set/get the camera view up direction. The view up vector will be
    /// normalized.
    void setViewUp(const glm::vec3 &up);
    glm::vec3 getViewUp() const { return m_viewUp; }
    //@}

    //@{
    /// @brief Set/Get the background color of the camera.
    /// @param color the background color
    void setBackgroundColor(const Color3f &color) { m_background = color; }
    Color3f getBackgroundColor() const { return m_background; }
    //@}

    //@{
    /// @brief Set/Get the aperature radius of the camera.
    /// @param radius the aperature radius
    void setApertureRadius(const float radius) { m_apertureRadius = radius; }
    float getApertureRadius() const { return m_apertureRadius; }
    //@}

    /// @brief The viewing transformation for moving objects from world space to view space.
    /// @return world to view transformation matrix
    glm::mat4 getViewMatrix();

    //@{
    /// Set/get the camera to world matrix for moving the camera to world space.
    glm::mat4 getCameraToWorldMatrix() const { return m_modelMatrix; }
    void setCameraToWorldMatrix(const glm::mat4 &matrix);
    //@}

    /// @brief Sample a stratified point in a unit square.
    /// @param i the current sample index in x direction
    /// @param j the current sample index in y direction
    /// @param spp the number of samples per pixel
    glm::vec2 sampleSquareStratified(const int i, const int j, const int spp) const;

    /// @brief Generate a Halton sequence of points in the range [0,1).
    /// @param N the number of points to generate
    /// @param b the base of the sequence
    /// @param out the output array of points
    static void generateHaltonSequence(const int N, const int b, float *out);

private:
    glm::vec3 m_position;
    glm::vec3 m_focalPoint;
    glm::vec3 m_viewUp;

    Color3f m_background;

    float     m_apertureRadius;
    glm::mat4 m_modelMatrix;
};
} // namespace raytracer

#endif