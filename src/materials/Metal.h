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
    Metal(const glm::vec3 &albedo, float roughness = 0.0f);

    /// @brief Determines if the ray scatters when it hits the object.
    /// @see Material::scatter
    bool scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered) const override;
private:
    glm::vec3 m_albedo;
    float m_roughness;
};
} // namespace raytracer