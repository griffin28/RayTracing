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
              const Color3d &color,
              double intensity);

    /// @brief a constructor to create a quad light with a quad, texture, and intensity.
    /// @param quad the quad that represents the light
    /// @param texture the texture of the light
    /// @param intensity the intensity of the light
    QuadLight(const std::shared_ptr<Quad> &quad,
              const std::shared_ptr<Texture> &texture,
              double intensity);

    /// @brief the destructor for the quad light.
    virtual ~QuadLight() = default;

    /// @see Shape::hit
    bool hit(const Ray &ray, HitRecord &record) const override;

    /// @see Shape::getBounds
    AxisAlignedBoundingBox getBounds() const override;

    /// @see Shape::center
    glm::dvec3 center() const override;

private:
    std::shared_ptr<Quad> m_quad;
    std::shared_ptr<EmissiveMaterial> m_material;
};
} // namespace raytracer