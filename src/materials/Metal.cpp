#include "Metal.h"
#include "Hittable.h"
#include "Ray.h"
#include "Utility.h"

#include <glm/glm.hpp>

namespace raytracer
{
//----------------------------------------------------------------------------------
Metal::Metal()
    : m_albedo(1.0f, 1.0f, 1.0f)
    , m_roughness(0.0f)
{
}

//----------------------------------------------------------------------------------
Metal::Metal(const glm::vec3 &albedo, float roughness)
    : m_albedo(albedo)
    , m_roughness(roughness < 1.0f ? roughness : 1.0f)
{
}

//----------------------------------------------------------------------------------
bool Metal::scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered) const
{
    glm::vec3 reflected = glm::reflect(glm::normalize(ray.direction()), record.normal);
    scattered = Ray(record.point, reflected + m_roughness * glm::normalize(randomInUnitSphere()));
    attenuation = m_albedo;

    // The scattered ray is reflected if the dot product of the scattered ray and the normal is greater than zero.
    // This way can absorbed rays that scatter below the surface of the object.
    return (dot(scattered.direction(), record.normal) > 0.0f);
}
}   // namespace raytracer