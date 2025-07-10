#pragma once

#include "Hittable.h"

namespace raytracer
{
/// @class Quad
/// @brief A class to represent a quad in the scene.
class Quad : public Hittable
{
public:
    /// @brief Default constructor for the quad.
    Quad();

    /// @brief a constructor to create a quad with a point and two vectors.
    /// @param Q the point on the quad
    /// @param u the first vector of the quad
    /// @param v the second vector of the quad
    /// @param material the material of the quad
    Quad(const glm::vec3 &Q,
         const glm::vec3 &u,
         const glm::vec3 &v,
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

    /// @brief Get the center of the quad in world space
    /// @see Hittable::center
    glm::vec3 center() const override;

private:
    glm::vec3 getQ() const;
    glm::vec3 getU() const;
    glm::vec3 getV() const;

    glm::vec3 getN() const;
    glm::vec3 getW() const;
    double getD() const;

    glm::vec3 m_Q;
    glm::vec3 m_u;
    glm::vec3 m_v;
    std::shared_ptr<Material> m_material;
};
} // namespace raytracer