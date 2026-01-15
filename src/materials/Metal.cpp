#include "Metal.h"
#include "SolidColorTexture.h"
#include "OrthoNormalBasis.h"
#include "Hittable.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
Metal::Metal()
    : m_albedo(std::make_shared<SolidColorTexture>(Color3f(1.0f, 1.0f, 1.0f)))
    , m_roughness(0.0f)
{
}

//----------------------------------------------------------------------------------
Metal::Metal(const Color3f &albedo, float roughness)
    : m_albedo(std::make_shared<SolidColorTexture>(albedo))
    , m_roughness(roughness < 1.0f ? roughness : 1.0f)
{
}

//----------------------------------------------------------------------------------
Metal::Metal(std::shared_ptr<Texture> albedo, float roughness)
    : m_albedo(albedo)
    , m_roughness(roughness < 1.0f ? roughness : 1.0f)
{
}

//----------------------------------------------------------------------------------
bool Metal::scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered, float &pdf) const
{    
    glm::vec3 reflected = glm::reflect(ray.direction(), record.normal);
    scattered = Ray(record.point, glm::normalize(reflected) + (m_roughness * RaytracingUtility::randomUnitVector()));
    attenuation = m_albedo->value(record.u, record.v, record.point);

    // The scattered ray is reflected if the dot product of the scattered ray and the normal is greater than zero.
    // This way can absorbed rays that scatter below the surface of the object.
    pdf = 1.0f;
    return (glm::dot(scattered.direction(), record.normal) > 0.0f);
}

//----------------------------------------------------------------------------------
float Metal::scatteringPDF(const Ray &ray, const HitRecord &record, const Ray &scattered) const
{
    return 1.0f;
}
}   // namespace raytracer