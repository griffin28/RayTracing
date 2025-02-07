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
    /// @param albedo the color of the material
    Lambertian(const Color3d &albedo);

    /// @brief Constructor
    /// @param albedo the texture of the material
    Lambertian(const std::shared_ptr<Texture> &albedo);

    bool scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const override;

private:
    std::shared_ptr<Texture> m_albedo;
};
} // namespace raytracer