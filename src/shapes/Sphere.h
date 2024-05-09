#pragma once

#include "Hittable.h"
#include "Material.h"

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
    Sphere(const glm::dvec3 &center, const double radius, std::shared_ptr<Material> material)
        : m_center(center)
        , m_radius(radius)
        , m_material(material)
        , m_isMoving(false) {}

    /// @brief a constructor to create a sphere with a center and radius.
    /// @param center the center of the sphere
    /// @param center2 the center of the sphere at time 1
    /// @param radius the radius of the sphere
    /// @param material the material of the sphere
    Sphere(const glm::dvec3 &center,
           const glm::dvec3 &center2,
           const double radius,
           std::shared_ptr<Material> material)
        : m_center(center)
        , m_radius(radius)
        , m_material(material)
        , m_isMoving(true) { m_centerVec = center2 - center; }

    /// @brief the center of the sphere at a given time.
    /// @param time the time to get the center of the sphere.
    /// @return  the position of the center at a certain time.
    glm::dvec3 center(const double time) const { return m_center + m_centerVec * time; }

    /// @brief Determines if the ray intersects the sphere.
    /// @see Hittable::hit
    bool hit(const Ray &ray, HitRecord &record) const override;

private:
    glm::dvec3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;
    bool m_isMoving;
    glm::dvec3 m_centerVec;
};
} // namespace raytracer