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
    virtual bool scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered, float &pdf) const = 0;

    /// @brief Computes the scattering PDF for the material.
    /// @param ray the ray that hit the object
    /// @param record the hit record that contains the intersection information
    /// @param scattered the scattered ray
    /// @return the scattering PDF
    virtual float scatteringPDF(const Ray &ray, const HitRecord &record, const Ray &scattered) const { return 1.0f; }

    /// @brief A light emitting material will emit light.
    /// @param record the hit record that contains the intersection information
    /// @return the emitted color
    virtual Color3f emitted(const HitRecord &record) const { return Color3f(0,0,0); }

    /// @brief Determines if the vector is close to zero in all dimensions.
    /// @param vec the vector to test
    /// @return true if the vector is close to zero in all dimensions, false otherwise
    bool nearZero(const glm::vec3 &vec) const
    {
        // Return true if the vector is close to zero in all dimensions.
        const float s = 1e-8;
        return (std::fabs(vec.x) < s) && (std::fabs(vec.y) < s) && (std::fabs(vec.z) < s);
    }

    /// @brief Reflects the vector about the normal.
    /// @param vec the vector to reflect
    /// @param normal the normal to reflect about
    /// @return the reflected vector
    glm::vec3 reflect(const glm::vec3 &vec, const glm::vec3 &normal) const
    {
        return vec - 2.0f * glm::dot(vec, normal) * normal;
    }
};
} // namespace raytracer