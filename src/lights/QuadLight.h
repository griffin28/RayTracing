#pragma once

#include "Hittable.h"

namespace raytracer
{
class Quad;
class Texture;
class EmissiveMaterial;

/// @class QuadLight
/// @brief A class to represent a quad light in the scene.
class QuadLight : public Hittable
{
public:
    QuadLight() = delete;

    /// @brief a constructor to create a quad light with a quad, color, and intensity.
    /// @param quad the quad that represents the light
    /// @param color the color of the light
    /// @param intensity the intensity of the light
    QuadLight(const std::shared_ptr<Quad> &quad,
              const Color3f &color,
              float intensity);

    /// @brief a constructor to create a quad light with a quad, texture, and intensity.
    /// @param quad the quad that represents the light
    /// @param texture the texture of the light
    /// @param intensity the intensity of the light
    QuadLight(const std::shared_ptr<Quad> &quad,
              const std::shared_ptr<Texture> &texture,
              float intensity);

    /// @brief the destructor for the quad light.
    virtual ~QuadLight() = default;

    /// @see Hittable::hit
    bool hit(const Ray &ray, HitRecord &record) const override;

    /// @see Hittable::getBounds
    AxisAlignedBoundingBox getBounds() const override;

    /// @see Hittable::center
    glm::vec3 center() const override;

    /// @see Hittable::isLight
    bool isLight() const override { return true; }
    
    /// @see Hittable::randomPointOnSurface
    glm::vec3 randomPointOnSurface() const override;

    /// @see Hittable::getSurfaceArea
    float getSurfaceArea() const override;

    /// @see Hittable::getGeometricFactor
    float getGeometricFactor(const glm::vec3 &origin, const glm::vec3 &direction) const override;

    /// @see Hittable::pdfValue
    float pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const override;

    /// @see Hittable::random
    glm::vec3 random(const glm::vec3 &origin) const override;

    /// @brief Get world point of quad corners
    /// @param index the corner index (0-3)
    /// @return the world coordinates of the specified corner
    glm::vec3 getCorner(int index) const;

private:
    std::shared_ptr<Quad> m_quad;
    std::shared_ptr<EmissiveMaterial> m_material;
};
} // namespace raytracer