#pragma once

#include "Material.h"

#include <memory>

namespace raytracer
{
class Texture;

/// @class EmissiveMaterial
/// @brief A light emitting material
class EmissiveMaterial : public Material
{
public:
    /// @brief Constructor
    /// @param color the color of the light
    /// @param intensity the intensity of the light
    EmissiveMaterial(const Color3f &color, float intensity = 1.f);

    /// @brief Constructor
    /// @param texture the texture of the light
    /// @param intensity the intensity of the light
    EmissiveMaterial(const std::shared_ptr<Texture> &texture, float intensity = 1.f);

    /// @brief Scatters the ray
    /// @see Material::scatter
    bool scatter(const Ray &ray, const HitRecord &record, ScatterRecord &scatterRecord) const override
    {
        return false;
    }

    /// @brief Emits the light
    /// @see Material::emitted
    Color3f emitted(const HitRecord &record) const override;

private:
    std::shared_ptr<Texture> m_texture;
    float m_intensity;
};
} // namespace raytracer