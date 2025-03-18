#pragma once

#include "Material.h"
#include "Texture.h"

#include <memory>

namespace raytracer
{
/// @class EmissiveMaterial
/// @brief A light emitting material
class EmissiveMaterial : public Material
{
public:
    /// @brief Constructor
    /// @param color the color of the light
    /// @param intensity the intensity of the light
    EmissiveMaterial(const Color3d &color, double intensity = 1.0);

    /// @brief Constructor
    /// @param texture the texture of the light
    /// @param intensity the intensity of the light
    EmissiveMaterial(const std::shared_ptr<Texture> &texture, double intensity = 1.0);

    /// @brief Scatters the ray
    /// @see Material::scatter
    bool scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const override
    {
        return false;
    }

    /// @brief Emits the light
    /// @see Material::emitted
    Color3d emitted(double u, double v, const glm::dvec3 &point) const override;

private:
    std::shared_ptr<Texture> m_texture;
    double m_intensity;
};
} // namespace raytracer