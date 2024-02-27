#pragma once

#include "Material.h"

namespace raytracer
{
class Dielectric : public Material
{
public:
    /// @brief a constructor to create a metal material with an albedo and roughness.
    Dielectric();

    /// @brief a constructor to create a metal material with an albedo and roughness.
    /// @param albedo the albedo of the metal
    /// @param eta the index of refraction
    /// @param roughness the roughness of the metal
    Dielectric(const glm::vec3 &albedo, float eta = 1.0f, float roughness = 0.0f);

    /// @brief Determines if the ray scatters when it hits the object.
    /// @see Material::scatter
    bool scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered) const override;

    /// @brief  Refracts the vector about the normal.
    /// @param uv the vector to refract
    /// @param n the normal to refract about
    /// @param etai_over_etat the ratio of the indices of refraction
    /// @return the refracted vector
    glm::vec3 refract(const glm::vec3 &uv, const glm::vec3 &n, float etai_over_etat) const;
private:
    glm::vec3 m_albedo;
    float m_roughness;
    float m_eta;
};
} // namespace raytracer