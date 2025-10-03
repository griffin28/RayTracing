#pragma once

#include "Hittable.h"
#include "Sphere.h"

namespace raytracer
{
class Texture;
class EmissiveMaterial;

/// @class SphereLight
/// @brief A class to represent a sphere light in the scene.
class SphereLight : public Hittable
{
public:
    SphereLight() = delete;

    /// @brief a constructor to create a sphere light with a sphere, color, and intensity.
    /// @param sphere the sphere that represents the light
    /// @param color the color of the light
    /// @param intensity the intensity of the light
    SphereLight(const std::shared_ptr<Sphere> &sphere,
                const Color3f &color,
                float intensity);

    /// @brief a constructor to create a sphere light with a sphere, texture, and intensity.
    /// @param sphere the sphere that represents the light
    /// @param texture the texture of the light
    /// @param intensity the intensity of the light
    SphereLight(const std::shared_ptr<Sphere> &sphere,
                const std::shared_ptr<Texture> &texture,
                float intensity);

    /// @brief the destructor for the sphere light.
    virtual ~SphereLight() = default;

    /// @see Shape::hit
    bool hit(const Ray &ray, HitRecord &record) const override;

    /// @see Shape::getBounds
    AxisAlignedBoundingBox getBounds() const override;

    /// @see Shape::center
    glm::vec3 center() const override;
    
    /// @see Hittable::randomPointOnSurface
    glm::vec3 randomPointOnSurface(float &surfaceArea) const override;

    /// @see Hittable::isLight
    bool isLight() const override { return true; }

    /// @brief Get the radius of the sphere light
    /// @return the radius of the sphere light
    float radius() const { return m_sphere->radius(); }

private:
    std::shared_ptr<Sphere> m_sphere;
    std::shared_ptr<EmissiveMaterial> m_material;
};
} // namespace raytracer