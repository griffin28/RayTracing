#include "Dielectric.h"
#include "Hittable.h"
#include "Ray.h"
#include "Utility.h"

#include <glm/glm.hpp>

namespace raytracer
{
//----------------------------------------------------------------------------------
Dielectric::Dielectric()
    : m_albedo(1.0, 1.0, 1.0)
    , m_eta(1.0)
    , m_roughness(0.0)

{
}

//----------------------------------------------------------------------------------
Dielectric::Dielectric(const glm::dvec3 &albedo, double eta, double roughness)
    : m_albedo(albedo)
    , m_eta(eta)
    , m_roughness(roughness < 1.0 ? roughness : 1.0)
{
}

//----------------------------------------------------------------------------------
bool Dielectric::scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const
{
    auto refractionRatio = record.frontFace ? (1.0 / m_eta) : m_eta;
    auto unitDirection = glm::normalize(ray.direction());
    auto cosTheta = fmin(glm::dot(-unitDirection, record.normal), 1.0);
    auto sinTheta = static_cast<double>(std::sqrt(1.0 - cosTheta * cosTheta));

    bool cannotRefract = refractionRatio * sinTheta > 1.0;
    glm::vec3 direction;

    if(cannotRefract)
    {
        direction = glm::reflect(unitDirection, record.normal);
    }
    else
    {
        direction = refract(unitDirection, record.normal, refractionRatio);
    }

    scattered = Ray(record.point, direction);
    attenuation = m_albedo;

    return true;
}

//----------------------------------------------------------------------------------
glm::dvec3 Dielectric::refract(const glm::dvec3 &uv, const glm::dvec3 &n, double etai_over_etat) const
{
    double cos_theta = fmin(glm::dot(-uv, n), 1.0);
    glm::dvec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    glm::dvec3 r_out_parallel = n * static_cast<double>(-std::sqrt(std::fabs(1.0 - glm::length(r_out_perp) * glm::length(r_out_perp))));
    return r_out_perp + r_out_parallel;
}
}   // namespace raytracer