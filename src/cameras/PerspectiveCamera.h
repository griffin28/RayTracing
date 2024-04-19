#ifndef INCLUDED_PERSPECTIVE_CAMERA_H
#define INCLUDED_PERSPECTIVE_CAMERA_H

#include "ProjectionCamera.h"
#include "HittableList.h"

namespace raytracer
{

/// @class PerspectiveCamera
/// @brief Represents a projection camera for perspective projections
///
/// The perspective camera projects a volume of space onto a 2D plane. It also uses the effect
/// of foreshortening where objects that are far away are projected to be smaller than objects
/// of the same size that are closer. The perspective projection doesn't preserve distances or
/// angles and parallel lines no longer remain parallel. The perspective projection is a reasonably
/// close match to how an eye or camera lens generates images of the 3D world.
class PerspectiveCamera : public ProjectionCamera
{
public:
    /// Default constructor.
    PerspectiveCamera();

    /// @brief Constructor
    /// @param width screen width
    /// @param height screen height
    /// @param maxDepth the maximum number of ray bounces into the scene
    /// @param fovy the view angle in degrees
    /// @param near the distance to the near clipping plane along the forward axis
    /// @param far the distance to the far clipping plane along the forward axis
    /// @throw std::invalid_argument if the screen size is invalid
    PerspectiveCamera(int width,
                      int height,
                      int maxDepth=10,
                      double fovy=45.0,
                      double near=0.0,
                      double far=1000.0);

    /// Default destructor.
    ~PerspectiveCamera() = default;

    /// @see Camera::reset
    void reset() override;

    /// @brief Renders the scene to the specified output stream.
    /// @param world the hittable list representing the scene
    /// @param samplesPerPixel the number of samples per pixel.
    /// @param out the output stream to write the rendered image to (default is std::cout)
    void render(const HittableList &world, const int samplesPerPixel=1, std::ostream &out=std::cout);

    /// @brief Creates a ray in world space from a screen pixel location. Caller is responsible
    ///        for managing the memory allocated for this object.
    ///        Implementation based on: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays.html
    /// @param p the x- and y-coordinates of the pixel in raster space
    /// @return the generated ray
    Ray *generateRay(const glm::dvec2 &p) override;

    /// @brief Creates a ray in world space from a screen pixel location and a lens offset to simulate
    ///        depth of field. Caller is responsible for managing the memory allocated for this object.
    /// @param pixel the x- and y-coordinates of the pixel in raster space
    /// @return the generated ray
    Ray *generateThinLensRay(const glm::dvec2 &pixel);

    /// @brief Change the view angle by the specified factor
    /// @param factor
    void zoom(const double factor) override;
    double getZoomFactor() const override { return m_zoomFactor; }

    //@{
    /// @brief Set/get the camera view angle.
    /// @param angle angular height of the camera view measured in degrees
    void setViewAngle(const double angle) override;
    double getViewAngle() const override { return m_fovy; }
    //@}

    //@{
    /// @brief Set/get the location of the near and far clipping planes along the forward axis
    ///        direction. Both of these values must be positive. The default clipping range is
    ///        (0.1,1000) and is measured in world coordinates.
    /// @param  near distance to the near clipping plane along the forward axis
    /// @param  far  distance to the far clipping plane along the forward axis
    void setClippingRange(const double near, const double far) override;
    glm::dvec2 getClippingRange() const override { return glm::dvec2(m_near, m_far); }
    //@}

    /// @brief Update the screen size.
    /// @param width screen width
    /// @param height screen height
    void setScreenSize(const int width, const int height) override;
    glm::dvec2 getScreenSize() const override { return glm::dvec2(m_width, m_height); }

    /// @brief Get the projection matrix in the form of a frustrum matrix such that clip-space
    ///        takes the shape of a truncated rectangular pyramid.
    /// @return the projection matrix
    glm::dmat4 getProjectionMatrix() const override { return m_perspectiveMatrix; }

    /// @brief Copy the camera's state to this camera.
    /// @see ProjectionCamera::copy
    void copy(const ProjectionCamera * const camera) override;

// private:
    /// @brief Compute the color of a ray.
    /// @param ray the ray to compute the color for
    /// @param depth the maximum number of ray bounces into the scene
    /// @param world the hittable list representing the scene
    glm::dvec3 rayColor(Ray * const ray, int depth, const HittableList &world);

    /// @brief Write a single pixel's color to the output stream.
    /// @param out the output stream
    /// @param pixelColor the color of the pixel (r,g,b) ranging from 0.0 to 1.0
    /// @param samplesPerPixel the number of samples per pixel
    void writeColor3(std::ostream& out, glm::dvec3 pixelColor, const int samplesPerPixel=1);

    int m_width;
    int m_height;
    int m_maxDepth;

    double m_zoomFactor;

    double m_fovy;
    double m_near;
    double m_far;

    glm::dmat4 m_perspectiveMatrix;
};
} // namespace raytracerS

#endif