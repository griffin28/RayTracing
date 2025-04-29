#pragma once

#include "Hittable.h"

namespace raytracer
{
/// @class Quad
/// @brief A class to represent a quad in the scene.
class Quad : public Hittable
{
public:
    Quad() = delete;

    /// @brief a constructor to create a quad with a point and two vectors.
    /// @param Q the point on the quad
    /// @param u the first vector of the quad
    /// @param v the second vector of the quad
    /// @param material the material of the quad
    Quad(const glm::dvec3 &Q,
         const glm::dvec3 &u,
         const glm::dvec3 &v,
         std::shared_ptr<Material> material = nullptr);

    /// @brief the destructor for the quad.
    virtual ~Quad() = default;

    /// @brief Set the material for the quad
    /// @param material the material to set
    void setMaterial(std::shared_ptr<Material> material) { m_material = material; }

    /// @brief Get the material for the quad
    /// @return the material for the quad
    Material *getMaterial() const { return m_material.get(); }

    /// @brief Determines if the ray intersects the quad.
    /// @see Hittable::hit
    bool hit(const Ray &ray, HitRecord &record) const override;

    /// @brief Get the world space bounds for this quad
    /// @see Hittable::getBounds
    AxisAlignedBoundingBox getBounds() const override;

    /// @brief Get the center of the quad
    /// @see Hittable::center
    glm::dvec3 center() const override;

private:
    glm::dvec3 getQ() const;
    glm::dvec3 getU() const;
    glm::dvec3 getV() const;

    glm::dvec3 getNormal() const;
    glm::dvec3 getW() const;
    double getD() const;

    glm::dvec3 m_Q;
    glm::dvec3 m_u;
    glm::dvec3 m_v;
    std::shared_ptr<Material> m_material;
};
} // namespace raytracer