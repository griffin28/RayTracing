#include "Lambertian.h"
#include "Hittable.h"
#include "Ray.h"
#include "Utility.h"

#include <glm/glm.hpp>

namespace raytracer
{
//----------------------------------------------------------------------------------
Lambertian::Lambertian()
    : m_albedo(0.0, 0.0, 0.0)
{
}

//----------------------------------------------------------------------------------
Lambertian::Lambertian(const glm::dvec3 &albedo)
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
    attenuation = m_albedo;
    return true;
}
}   // namespace raytracer