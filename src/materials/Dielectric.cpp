#include "Dielectric.h"
#include "Hittable.h"
#include "Ray.h"
#include "Utility.h"

#include <glm/glm.hpp>

namespace raytracer
{
//----------------------------------------------------------------------------------
Dielectric::Dielectric()
    : m_albedo(1.0f, 1.0f, 1.0f)
    , m_eta(1.0f)
    , m_roughness(0.0f)

{
}

//----------------------------------------------------------------------------------
Dielectric::Dielectric(const glm::vec3 &albedo, float eta, float roughness)
    : m_albedo(albedo)
    , m_eta(eta)
    , m_roughness(roughness < 1.0f ? roughness : 1.0f)
{
}

//----------------------------------------------------------------------------------
bool Dielectric::scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered) const
{
    auto refractionRatio = record.frontFace ? (1.0f / m_eta) : m_eta;
    auto unitDirection = glm::normalize(ray.direction());
    auto cosTheta = fmin(glm::dot(-unitDirection, record.normal), 1.0f);
    auto sinTheta = static_cast<float>(std::sqrt(1.0 - cosTheta * cosTheta));

    bool cannotRefract = refractionRatio * sinTheta > 1.0f;
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
glm::vec3 Dielectric::refract(const glm::vec3 &uv, const glm::vec3 &n, float etai_over_etat) const
{
    float cos_theta = fmin(glm::dot(-uv, n), 1.0);
    glm::vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    glm::vec3 r_out_parallel = n * static_cast<float>(-std::sqrt(std::fabs(1.0 - glm::length(r_out_perp) * glm::length(r_out_perp))));
    return r_out_perp + r_out_parallel;
}
}   // namespace raytracer