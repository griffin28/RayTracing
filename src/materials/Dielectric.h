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
    Dielectric(float indexOfRefraction);

    /// @brief Determines if the ray scatters when it hits the object.
    /// @see Material::scatter
    bool scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered, float &pdf) const override;

    /// @brief  Refracts the vector about the normal.
    /// @param uv the vector to refract
    /// @param n the normal to refract about
    /// @param etai_over_etat the ratio of the indices of refraction
    /// @return the refracted vector
    glm::vec3 refract(const glm::vec3 &uv, const glm::vec3 &n, float etai_over_etat) const;

    /// @brief  Use schlick's approximation for reflectance.
    /// @param cosine the cosine of the angle
    /// @param ref_idx the index of refraction
    float reflectance(float cosine, float ref_idx) const;
private:
    float m_ir;
};
} // namespace raytracer