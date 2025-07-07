#pragma once

#include "Material.h"
#include "Texture.h"
#include <memory>

namespace raytracer
{
/// @class Metal
/// @brief Metal material
/// A class to represent a metal material in the scene.
class Metal : public Material
{
public:
    /// @brief a constructor to create a metal material with an albedo and roughness.
    Metal();

    /// @brief a constructor to create a metal material with an albedo and roughness.
    /// @param albedo the albedo of the metal
    /// @param roughness the roughness of the metal
    Metal(const Color3f &albedo, float roughness = 0.0f);

    /// @brief a constructor to create a metal material with an albedo and roughness.
    /// @param albedo the albedo texture of the metal
    /// @param roughness the roughness of the metal
    Metal(std::shared_ptr<Texture> albedo, float roughness = 0.0f);

    /// @brief Determines if the ray scatters when it hits the object.
    /// @see Material::scatter
    bool scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered) const override;
private:
    std::shared_ptr<Texture> m_albedo;
    float m_roughness;
};
} // namespace raytracer