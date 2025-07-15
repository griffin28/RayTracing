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
    Sphere(const glm::vec3 &center,
           const float radius,
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
    glm::vec3 center() const override { return m_center; }

    /// @brief Determines if the ray intersects the sphere.
    /// @see Hittable::hit
    bool hit(const Ray &ray, HitRecord &record) const override;

    /// @brief Get the world space bounds for this sphere
    /// @return Bounding box for the sphere using world space coordinates
    AxisAlignedBoundingBox getBounds() const override { return m_bounds; }
    
    /// @brief translate the sphere in world space
    /// @param translation the coordinates, in world space, of a translation vector
    /// @note This function modifies the model matrix of the sphere.
    /// @see Hittable::translate
    void translate(const glm::vec3 &translation) override;
    
    /// @brief rotate the sphere in world space
    /// @param angle the angle to rotate in degrees
    /// @param axis rotation axis, recommended to be normalized
    /// @note This function modifies the model matrix of the sphere.
    /// @see Hittable::rotate
    void rotate(const float angle, const glm::vec3 &axis) override;
    
    /// @brief scale the sphere in world space
    /// @param scale ratio of scaling for each axis
    /// @note This function modifies the model matrix of the sphere.
    /// @see Hittable::scale
    void scale(const glm::vec3 &scale) override;

    /// @brief Get the texture coordinates of the sphere
    /// @param p the point on the sphere
    /// @param u the u texture coordinate
    /// @param v the v texture coordinate
    static void getSphereUV(const glm::vec3 &p, float &u, float &v)
    {
        auto theta = acos(-p.y);
        auto phi = atan2(-p.z, p.x) + glm::pi<float>();

        u = phi / (2.f * glm::pi<float>());
        v = theta / glm::pi<float>();
    }

private:
    void updateCenter();
    void updateBounds(); 

    glm::vec3 m_center;
    float m_radius;
    std::shared_ptr<Material> m_material;
    
    AxisAlignedBoundingBox m_bounds; // Cached bounds for performance
};
} // namespace raytracer