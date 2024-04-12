#pragma once

#include "Material.h"

namespace raytracer
{
class Dielectric : public Material
{
public:
    /// @brief a constructor to create a glass material.
    Dielectric();

    /// @brief a constructor to create a glass material with an index of refraction.
    /// @param indexOfRefraction the index of refraction
    Dielectric(double indexOfRefraction);

    /// @brief Determines if the ray scatters when it hits the object.
    /// @see Material::scatter
    bool scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const override;

    /// @brief  Refracts the vector about the normal.
    /// @param uv the vector to refract
    /// @param n the normal to refract about
    /// @param etai_over_etat the ratio of the indices of refraction
    /// @return the refracted vector
    glm::dvec3 refract(const glm::dvec3 &uv, const glm::dvec3 &n, double etai_over_etat) const;

    /// @brief  Use schlick's approximation for reflectance.
    /// @param cosine the cosine of the angle
    /// @param ref_idx the index of refraction
    double reflectance(double cosine, double ref_idx) const;
private:
    double m_ir;
};
} // namespace raytracer