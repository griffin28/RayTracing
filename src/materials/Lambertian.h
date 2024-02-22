#pragma once

#include "Material.h"

namespace raytracer
{
class Lambertian : public Material
{
public:
    Lambertian();
    Lambertian(const glm::vec3 &albedo);

    bool scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered) const override;

private:
    glm::vec3 m_albedo;
};
} // namespace raytracer