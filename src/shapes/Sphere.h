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
    Sphere(const glm::vec3 &center, const float radius)
        : m_center(center)
        , m_radius(radius) {}

    /// @brief Determines if the ray intersects the sphere.
    /// @see Hittable::hit
    bool hit(const Ray &ray, HitRecord &record) const override;

private:
    glm::vec3 m_center;
    float m_radius;
};
} // namespace raytracer