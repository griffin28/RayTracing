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
    Dielectric(const glm::dvec3 &albedo, double eta = 1.0, double roughness = 0.0);

    /// @brief Determines if the ray scatters when it hits the object.
    /// @see Material::scatter
    bool scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const override;

    /// @brief  Refracts the vector about the normal.
    /// @param uv the vector to refract
    /// @param n the normal to refract about
    /// @param etai_over_etat the ratio of the indices of refraction
    /// @return the refracted vector
    glm::dvec3 refract(const glm::dvec3 &uv, const glm::dvec3 &n, double etai_over_etat) const;
private:
    glm::dvec3 m_albedo;
    double m_roughness;
    double m_eta;
};
} // namespace raytracer