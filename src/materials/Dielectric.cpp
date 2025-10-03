#include "Dielectric.h"
#include "Hittable.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
Dielectric::Dielectric()
    : m_ir(1.0)
{
}

//----------------------------------------------------------------------------------
Dielectric::Dielectric(float indexOfRefraction)
    : m_ir(indexOfRefraction)
{
}

//----------------------------------------------------------------------------------
float Dielectric::scatteringPDF(const Ray &ray, const HitRecord &record, const Ray &scattered) const
{
    return 1.0f;
}

//----------------------------------------------------------------------------------
bool Dielectric::scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered, float &pdf) const
{
    attenuation = glm::vec3(1.0, 1.0, 1.0);
    auto refractionRatio = record.frontFace ? (1.0 / m_ir) : m_ir;
    auto unitDirection = glm::normalize(ray.direction());
    auto cosTheta = fmin(glm::dot(-unitDirection, record.normal), 1.0);
    auto sinTheta = static_cast<float>(std::sqrt(1.0 - cosTheta * cosTheta));

    bool cannotRefract = refractionRatio * sinTheta > 1.0;
    glm::vec3 direction;
    const float randomVal = static_cast<float>(RaytracingUtility::randomDouble());
    
    if(cannotRefract || reflectance(cosTheta, refractionRatio) > randomVal)
    {
        direction = glm::reflect(unitDirection, record.normal);
    }
    else
    {
        direction = refract(unitDirection, record.normal, refractionRatio);
    }

    pdf = 1.0f;
    scattered = Ray(record.point, direction);
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

//----------------------------------------------------------------------------------
float Dielectric::reflectance(float cosine, float ref_idx) const
{
    // Use Schlick's approximation for reflectance.
    auto r0 = (1.f - ref_idx) / (1.f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.f - r0) * static_cast<float>(std::pow((1.f - cosine), 5));
}
}   // namespace raytracer