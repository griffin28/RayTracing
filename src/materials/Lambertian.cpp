#include "Lambertian.h"
#include "SolidColorTexture.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
Lambertian::Lambertian()
    : m_albedo(std::make_shared<SolidColorTexture>(Color3d(1.0, 1.0, 1.0)))
{
}

//----------------------------------------------------------------------------------
Lambertian::Lambertian(const Color3d &albedo)
    : m_albedo(std::make_shared<SolidColorTexture>(albedo))
{
}

//----------------------------------------------------------------------------------
Lambertian::Lambertian(const std::shared_ptr<Texture> &albedo)
    : m_albedo(albedo)
{
}

//----------------------------------------------------------------------------------
bool Lambertian::scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const
{
    glm::dvec3 scatterDirection = record.normal + glm::normalize(randomInUnitSphere());

    // Catch degenerate scatter direction
    if(nearZero(scatterDirection))
    {
        scatterDirection = record.normal;
    }

    scattered = Ray(record.point, scatterDirection);
    attenuation = m_albedo->value(record.u, record.v, record.point);
    return true;
}
}   // namespace raytracer