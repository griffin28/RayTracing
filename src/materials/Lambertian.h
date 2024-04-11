#pragma once

#include "Material.h"

namespace raytracer
{
class Lambertian : public Material
{
public:
    Lambertian();
    Lambertian(const glm::dvec3 &albedo);

    bool scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const override;

private:
    glm::dvec3 m_albedo;
};
} // namespace raytracer