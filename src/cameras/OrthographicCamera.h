#ifndef INCLUDED_ORTHOGRAPHIC_CAMERA_H
#define INCLUDED_ORTHOGRAPHIC_CAMERA_H

#include "ProjectionCamera.h"

namespace raytracer
{
/// @class OrthographicCamera
/// @brief Represents a projection camera for parallel projections
///
/// The orthographic camera is commonly used in engineering. All objects appear at the same scale,
/// parallel lines remain parallel and a unit of length will appear to have the same length
/// everywhere in the rendering (it doesn't give the effect of foreshortening).
class OrthographicCamera : public ProjectionCamera
{
public:
    /// @brief Constructor
    /// @param width screen width
    /// @param height screen height
    /// @param fovDistance scalar field of view in meters
    /// @param near the near clipping plane along the forward axis
    /// @param far the far clipping plane along the forward axis
    OrthographicCamera(int width,
                       int height,
                       double fovDistance=100.0,
                       double near=0.0,
                       double far=1000.0);

    /// Default destructor.
    ~OrthographicCamera() = default;

    /// @see Camera::reset
    void reset() override;

    /// Creates a ray for an orthographic projection in world space from a screen pixel location.
    /// Caller is responsible for managing the memory allocated for this object.
    /// @param pixel the x- and y-coordinates of the pixel in raster space
    /// @return
    Ray *generateRay(const glm::dvec2 &pixel) override;

    /// @brief translate in the z-direction
    /// @param factor
    void zoom(const double factor) override;
    double getZoomFactor() const override { return m_zoomFactor; }

    /// @brief Set the camera view angle. This will have no effect for an orthographic camera as
    ///        the effective view angle is always zero degrees.
    /// @param angle angular height of the camera view measured in degrees
    void setViewAngle(const double angle) override { }
    double getViewAngle() const override { return 0.0; }

    //@{
    /// @brief Set/get the location of the near and far clipping planes along the forward axis
    ///        direction. Both of these values must be positive. The default clipping range is
    ///        (0.1,1000) and is measured in world coordinates.
    /// @param  near the near clipping plane along the forward axis
    /// @param  far  the far clipping plane along the forward axis
    void setClippingRange(const double near, const double far) override;
    glm::dvec2 getClippingRange() const override { return glm::dvec2(m_near, m_far); }
    //@}

    /// @brief Update the screen size.
    /// @param width screen width
    /// @param height screen height
    void setScreenSize(const int width, const int height) override;
    glm::dvec2 getScreenSize() const override { return glm::dvec2(m_width, m_height); }

    /// @brief Get the parallel projection matrix.
    /// @return the parallel projection matrix
    glm::dmat4 getProjectionMatrix() const override { return m_orthographicMatrix; }

    //@{
    /// @brief Set/get the scalar field of view distance in meters?.
    /// @param  distance scalar field of view in meters
    void setFOVDistance(const double distance) { m_fovDistance = distance; }
    double getFOVDistance() const { return m_fovDistance; }
    //@}

    /// Copy the camera's state to this camera
    /// @see ProjectionCamera::copy
    void copy(const ProjectionCamera * const camera) override;

private:
    int m_width;
    int m_height;
    double m_scale;
    double m_zoomFactor;

    double m_fovDistance;
    double m_near;
    double m_far;

    glm::dmat4 m_orthographicMatrix;
};
} // namespace raytracer

#endif