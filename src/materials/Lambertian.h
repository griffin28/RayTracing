#pragma once

#include "Material.h"
#include "Texture.h"
#include <memory>

namespace raytracer
{

/// @class Lambertian
/// @brief Lambertian material
class Lambertian : public Material
{
public:
    /// @brief Constructor
    /// Creates a white lambertian material.
    Lambertian();

    /// @brief Constructor
    /// @param albedo the color of the material
    Lambertian(const Color3f &albedo);

    /// @brief Constructor
    /// @param albedo the texture of the material
    Lambertian(const std::shared_ptr<Texture> &albedo);

    /// @brief  Determines if the ray scatters when it hits the object.
    /// @see Material::scatter
    bool scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered, float &pdf) const override;

    /// @brief Computes the scattering PDF for the material.
    /// @see Material::scatteringPDF
    float scatteringPDF(const Ray &ray, const HitRecord &record, const Ray &scattered) const override;

private:
    std::shared_ptr<Texture> m_albedo;
};
} // namespace raytracer