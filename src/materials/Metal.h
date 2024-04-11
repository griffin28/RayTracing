#pragma once

#include "Material.h"

namespace raytracer
{
class Metal : public Material
{
public:
    /// @brief a constructor to create a metal material with an albedo and roughness.
    Metal();

    /// @brief a constructor to create a metal material with an albedo and roughness.
    /// @param albedo the albedo of the metal
    /// @param roughness the roughness of the metal
    Metal(const glm::dvec3 &albedo, double roughness = 0.0);

    /// @brief Determines if the ray scatters when it hits the object.
    /// @see Material::scatter
    bool scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const override;
private:
    glm::dvec3 m_albedo;
    double m_roughness;
};
} // namespace raytracer