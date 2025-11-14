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
    Box(const glm::vec3 &a,
        const glm::vec3 &b,
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

    /// @brief the center of the box at a given time in world space
    /// @note This function takes into account the model matrix of the box.
    /// @see Hittable::center
    /// @return  the position of the center at a certain time.
    glm::vec3 center() const override;

    /// @brief Get the world space bounds for this box
    /// @return Bounding box for the box using world space coordinates
    AxisAlignedBoundingBox getBounds() const override;
    
    /// @brief translate the box in world space
    /// @param translation the coordinates, in world space, of a translation vector
    /// @note This function modifies the model matrix of the box.
    /// @see Hittable::translate
    void translate(const glm::vec3 &translation) override;
    
    /// @brief rotate the box in world space
    /// @param angle the angle to rotate in degrees
    /// @param axis rotation axis, recommended to be normalized
    /// @note This function modifies the model matrix of the box.
    /// @see Hittable::rotate
    void rotate(const float angle, const glm::vec3 &axis) override;
    
    /// @brief scale the box in world space
    /// @param scale ratio of scaling for each axis
    /// @note This function modifies the model matrix of the box.
    /// @see Hittable::scale
    void scale(const glm::vec3 &scale) override;
    
    /// @brief Get the sides of the box
    /// @return the sides of the box
    const std::vector<std::shared_ptr<Quad>> &getSides() const { return m_sides; }
    
    /// @brief Get the first point of the box in world space
    /// @note This function takes into account the model matrix of the box. 
    /// @return the first point of the box
    glm::vec3 getWorldPoint1() const;

    /// @brief Get the second point of the box in world space
    /// @note This function takes into account the model matrix of the box.
    /// @return the second point of the box
    glm::vec3 getWorldPoint2() const;    

    /// @see Hittable::randomPointOnSurface
    glm::vec3 randomPointOnSurface(float &surfaceArea) const override;

    /// @see Hittable::pdfValue
    float pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const override;

    /// @see Hittable::random
    glm::vec3 random(const glm::vec3 &origin) const override;

private:
    void createSides();
    
    glm::vec3 m_point1;
    glm::vec3 m_point2;
    std::shared_ptr<Material> m_material;

    std::vector<std::shared_ptr<Quad>> m_sides;
};
} // namespace raytracer