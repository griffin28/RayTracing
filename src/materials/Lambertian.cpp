#include "Lambertian.h"
#include "SolidColorTexture.h"
#include "Hittable.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
Lambertian::Lambertian()
    : m_albedo(std::make_shared<SolidColorTexture>(Color3f(1.0f, 1.0f, 1.0f)))
{
}

//----------------------------------------------------------------------------------
Lambertian::Lambertian(const Color3f &albedo)
    : m_albedo(std::make_shared<SolidColorTexture>(albedo))
{
}

//----------------------------------------------------------------------------------
Lambertian::Lambertian(const std::shared_ptr<Texture> &albedo)
    : m_albedo(albedo)
{
}

//----------------------------------------------------------------------------------
bool Lambertian::scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered) const
{
    glm::vec3 scatterDirection = record.normal + glm::normalize(randomInUnitSphere());

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