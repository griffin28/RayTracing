#include "Metal.h"
#include "Hittable.h"
#include "Ray.h"
#include "Utility.h"

#include <glm/glm.hpp>

namespace raytracer
{
//----------------------------------------------------------------------------------
Metal::Metal()
    : m_albedo(1.0, 1.0, 1.0)
    , m_roughness(0.0)
{
}

//----------------------------------------------------------------------------------
Metal::Metal(const glm::dvec3 &albedo, double roughness)
    : m_albedo(albedo)
    , m_roughness(roughness < 1.0 ? roughness : 1.0)
{
}

//----------------------------------------------------------------------------------
bool Metal::scatter(const Ray &ray, const HitRecord &record, glm::dvec3 &attenuation, Ray &scattered) const
{
    glm::dvec3 reflected = glm::reflect(glm::normalize(ray.direction()), record.normal);
    scattered = Ray(record.point, reflected + m_roughness * glm::normalize(randomInUnitSphere()));
    attenuation = m_albedo;

    // The scattered ray is reflected if the dot product of the scattered ray and the normal is greater than zero.
    // This way can absorbed rays that scatter below the surface of the object.
    return (dot(scattered.direction(), record.normal) > 0.0);
}
}   // namespace raytracer