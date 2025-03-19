#pragma once

#include "Ray.h"
#include "Utility.h"

#include <cmath>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

namespace raytracer
{
struct HitRecord;

/// @class Material
/// @brief A material is an object that can scatter light.
class Material
{
public:
    /// @brief Destructor
    virtual ~Material() = default;

    /// @brief Determines if the ray scatters when it hits the object.
    /// @param ray the ray that hit the object
    /// @param record the hit record that contains the intersection information
    /// @param attenuation the color attenuation
    /// @param scattered the scattered ray if the ray scatters
    /// @return true if the ray scatters, false otherwise
    virtual bool scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const = 0;

    /// @brief A light emitting material will emit light.
    /// @param u the u coordinate of the hit point
    /// @param v the v coordinate of the hit point
    /// @param point the point of the hit
    /// @return the emitted color
    virtual Color3d emitted(double u, double v, const glm::dvec3 &point) const { return Color3d(0,0,0); }

    /// @brief Determines if the vector is close to zero in all dimensions.
    /// @param vec the vector to test
    /// @return true if the vector is close to zero in all dimensions, false otherwise
    bool nearZero(const glm::dvec3 &vec) const
    {
        // Return true if the vector is close to zero in all dimensions.
        const float s = 1e-8;
        return (std::fabs(vec.x) < s) && (std::fabs(vec.y) < s) && (std::fabs(vec.z) < s);
    }

    /// @brief Reflects the vector about the normal.
    /// @param vec the vector to reflect
    /// @param normal the normal to reflect about
    /// @return the reflected vector
    glm::vec3 reflect(const glm::dvec3 &vec, const glm::dvec3 &normal) const
    {
        return vec - 2.0f * glm::dot(vec, normal) * normal;
    }
};
} // namespace raytracer