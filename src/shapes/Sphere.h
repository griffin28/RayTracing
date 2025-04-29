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
    /// @brief a default constructor to create a sphere.
    Sphere();

    /// @brief a constructor to create a sphere with a center and radius.
    /// @param center the center of the sphere
    /// @param radius the radius of the sphere
    /// @param material the material of the sphere
    Sphere(const glm::dvec3 &center,
           const double radius,
           std::shared_ptr<Material> material = nullptr);

    /// @brief the destructor for the sphere.
    virtual ~Sphere() = default;

    /// @brief Set the material for the sphere
    /// @param material the material to set
    void setMaterial(std::shared_ptr<Material> material) { m_material = material; }

    /// @brief Get the material for the sphere
    /// @return the material for the sphere
    Material *getMaterial() const { return m_material.get(); }

    /// @brief the center of the sphere at a given time.
    /// @return  the position of the center at a certain time.
    glm::dvec3 center() const override;

    /// @brief Determines if the ray intersects the sphere.
    /// @see Hittable::hit
    bool hit(const Ray &ray, HitRecord &record) const override;

    /// @brief Get the world space bounds for this sphere
    /// @return Bounding box for the sphere using world space coordinates
    AxisAlignedBoundingBox getBounds() const override;

    /// @brief Get the texture coordinates of the sphere
    /// @param p the point on the sphere
    /// @param u the u texture coordinate
    /// @param v the v texture coordinate
    static void getSphereUV(const glm::dvec3 &p, double &u, double &v)
    {
        auto theta = acos(-p.y);
        auto phi = atan2(-p.z, p.x) + glm::pi<double>();

        u = phi / (2 * glm::pi<double>());
        v = theta / glm::pi<double>();
    }

private:
    glm::dvec3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;
};
} // namespace raytracer