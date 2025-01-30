#pragma once

#include "Hittable.h"

#include <glm/vec3.hpp>

namespace raytracer
{
/// @class Sphere
/// @brief A class to represent a sphere in the scene.
class Sphere : public Hittable
{
public:
    Sphere() = default;

    /// @brief a constructor to create a sphere with a center and radius.
    /// @param center the center of the sphere
    /// @param radius the radius of the sphere
    /// @param material the material of the sphere
    Sphere(const glm::dvec3 &center,
           const double radius,
           std::shared_ptr<Material> material);

    /// @brief the center of the sphere at a given time.
    /// @return  the position of the center at a certain time.
    glm::dvec3 center() const override { return m_center; }

    /// @brief Determines if the ray intersects the sphere.
    /// @see Hittable::hit
    bool hit(const Ray &ray, HitRecord &record) const override;

    /// @brief Get the world space bounds for this sphere
    /// @return Bounding box for the sphere using world space coordinates
    AxisAlignedBoundingBox getBounds() const override { return m_bounds; }

private:
    glm::dvec3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;
    AxisAlignedBoundingBox m_bounds;
};
} // namespace raytracer