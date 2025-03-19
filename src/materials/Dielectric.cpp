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
Dielectric::Dielectric(double indexOfRefraction)
    : m_ir(indexOfRefraction)
{
}

//----------------------------------------------------------------------------------
bool Dielectric::scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const
{
    attenuation = glm::dvec3(1.0, 1.0, 1.0);
    auto refractionRatio = record.frontFace ? (1.0 / m_ir) : m_ir;
    auto unitDirection = glm::normalize(ray.direction());
    auto cosTheta = fmin(glm::dot(-unitDirection, record.normal), 1.0);
    auto sinTheta = static_cast<double>(std::sqrt(1.0 - cosTheta * cosTheta));

    bool cannotRefract = refractionRatio * sinTheta > 1.0;
    glm::vec3 direction;

    if(cannotRefract || reflectance(cosTheta, refractionRatio) > randomDouble())
    {
        direction = glm::reflect(unitDirection, record.normal);
    }
    else
    {
        direction = refract(unitDirection, record.normal, refractionRatio);
    }

    scattered = Ray(record.point, direction);
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

//----------------------------------------------------------------------------------
double Dielectric::reflectance(double cosine, double ref_idx) const
{
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * static_cast<double>(std::pow((1 - cosine), 5));
}
}   // namespace raytracer