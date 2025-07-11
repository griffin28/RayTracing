#ifndef INCLUDED_PERSPECTIVE_CAMERA_H
#define INCLUDED_PERSPECTIVE_CAMERA_H

#include "ProjectionCamera.h"
#include "BVH.h"
#include "Utility.h"

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
                      float fovy=45.0,
                      float near=0.0,
                      float far=1000.0);

    /// Default destructor.
    ~PerspectiveCamera() = default;

    /// @see Camera::reset
    void reset() override;

    /// @brief Renders the scene to the specified output stream.
    /// @param world the hittable list representing the scene
    /// @param samplesPerPixel the number of samples per pixel.
    /// @param out the output stream to write the rendered image to (default is std::cout)
    void render(const BVH &world, const int samplesPerPixel=1, std::ostream &out=std::cout);

    /// @brief Creates a ray in world space from a screen pixel location. Caller is responsible
    ///        for managing the memory allocated for this object.
    ///        Implementation based on: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays.html
    /// @param p the x- and y-coordinates of the pixel in raster space
    /// @return the generated ray
    Ray *generateRay(const glm::vec2 &p) override;

    /// @brief Creates a ray in world space from a screen pixel location and a lens offset to simulate
    ///        depth of field. Caller is responsible for managing the memory allocated for this object.
    /// @param pixel the x- and y-coordinates of the pixel in raster space
    /// @return the generated ray
    Ray *generateThinLensRay(const glm::vec2 &pixel);

    /// @brief Change the view angle by the specified factor
    /// @param factor
    void zoom(const float factor) override;
    float getZoomFactor() const override { return m_zoomFactor; }

    //@{
    /// @brief Set/get the camera view angle.
    /// @param angle angular height of the camera view measured in degrees
    void setViewAngle(const float angle) override;
    float getViewAngle() const override { return m_fovy; }
    //@}

    //@{
    /// @brief Set/get the location of the near and far clipping planes along the forward axis
    ///        direction. Both of these values must be positive. The default clipping range is
    ///        (0.1,1000) and is measured in world coordinates.
    /// @param  near distance to the near clipping plane along the forward axis
    /// @param  far  distance to the far clipping plane along the forward axis
    void setClippingRange(const float near, const float far) override;
    glm::vec2 getClippingRange() const override { return glm::vec2(m_near, m_far); }
    //@}

    /// @brief Update the screen size.
    /// @param width screen width
    /// @param height screen height
    void setScreenSize(const int width, const int height) override;
    glm::vec2 getScreenSize() const override { return glm::vec2(m_width, m_height); }

    /// @brief Get the projection matrix in the form of a frustrum matrix such that clip-space
    ///        takes the shape of a truncated rectangular pyramid.
    /// @return the projection matrix
    glm::mat4 getProjectionMatrix() const override { return m_perspectiveMatrix; }

    /// @brief Copy the camera's state to this camera.
    /// @see ProjectionCamera::copy
    void copy(const ProjectionCamera * const camera) override;

private:
    /// @brief Compute the color of a ray.
    /// @param ray the ray to compute the color for
    /// @param depth the maximum number of ray bounces into the scene
    /// @param world the hittable list representing the scene
    Color3f rayColor(Ray * const ray, int depth, const BVH &world);

    /// @brief  Write a PPM image to the output stream.
    /// @param image PPM image data
    /// @param width the width of the image
    /// @param height the height of the image
    /// @param out the output stream
    void writePPMImage(uint8_t *image, const int width, const int height, std::ostream &out);

    int m_width;
    int m_height;
    int m_maxDepth;

    float m_zoomFactor;

    float m_fovy;
    float m_near;
    float m_far;

    glm::mat4 m_perspectiveMatrix;
};
} // namespace raytracerS

#endif