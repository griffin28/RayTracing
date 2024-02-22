#include "Metal.h"
#include "Hittable.h"
#include "Ray.h"
#include "Utility.h"

#include <glm/glm.hpp>

namespace raytracer
{
//----------------------------------------------------------------------------------
Metal::Metal()
    : m_albedo(0.0f, 0.0f, 0.0f)
{
}

//----------------------------------------------------------------------------------
Metal::Metal(const glm::vec3 &albedo)
    : m_albedo(albedo)
{
}

//----------------------------------------------------------------------------------
bool Metal::scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered) const
{
    glm::vec3 reflected = glm::reflect(glm::normalize(ray.direction()), record.normal);
    scattered = Ray(record.point, reflected);
    attenuation = m_albedo;
    return true;
}
}   // namespace raytracer