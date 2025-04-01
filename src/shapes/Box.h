#pragma once

#include "Hittable.h"

#include <glm/vec3.hpp>

namespace raytracer
{
class Quad;

/// @class Box
/// @brief A class to represent a box in the scene.
class Box : public Hittable
{
public:
    /// @brief no default constructor for the box.
    Box() = delete;

    /// @brief a constructor to create a box with two points.
    /// @param a the first point of the box
    /// @param b the second point of the box
    /// @param material the material of the box
    Box(const glm::dvec3 &a,
        const glm::dvec3 &b,
        std::shared_ptr<Material> material = nullptr);

    /// @brief the destructor for the box.
    virtual ~Box() = default;

     /// @brief Determines if the ray intersects the box.
    /// @see Hittable::hit
    bool hit(const Ray &ray, HitRecord &record) const override;

    /// @brief Set the material for the box
    /// @param material the material to set
    void setMaterial(std::shared_ptr<Material> material) { m_material = material; }

    /// @brief Get the material for the box
    /// @return the material for the box
    Material *getMaterial() const { return m_material.get(); }

    /// @brief the center of the box at a given time.
    /// @return  the position of the center at a certain time.
    glm::dvec3 center() const override { return m_center; }

    /// @brief Get the world space bounds for this box
    /// @return Bounding box for the box using world space coordinates
    AxisAlignedBoundingBox getBounds() const override { return m_bounds; }

private:
    glm::dvec3 m_point1;
    glm::dvec3 m_point2;
    std::shared_ptr<Material> m_material;

    glm::dvec3 m_center;
    AxisAlignedBoundingBox m_bounds;
    std::unique_ptr<Quad> m_sides[6];
};
} // namespace raytracer