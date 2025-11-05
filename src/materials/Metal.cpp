#include "Metal.h"
#include "SolidColorTexture.h"
#include "Hittable.h"
#include "OrthoNormalBasis.h"

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
    OrthoNormalBasis onb(record.normal);
    glm::vec3 scatterDirection = onb.transform(RaytracingUtility::randomCosineDirection());
    
    glm::vec3 reflected = glm::reflect(glm::normalize(ray.direction()), record.normal);
    scattered = Ray(record.point, reflected + m_roughness * glm::normalize(scatterDirection));
    attenuation = m_albedo->value(record.u, record.v, record.point);

    // The scattered ray is reflected if the dot product of the scattered ray and the normal is greater than zero.
    // This way can absorbed rays that scatter below the surface of the object.
    pdf = glm::dot(onb.w(), scattered.direction()) / glm::pi<float>();
    return (glm::dot(scattered.direction(), record.normal) > 0.0f);
}

//----------------------------------------------------------------------------------
float Metal::scatteringPDF(const Ray &ray, const HitRecord &record, const Ray &scattered) const
{
    // float cosineTheta = glm::dot(record.normal, glm::normalize(scattered.direction()));
    // return (cosineTheta < 0) ? 0 : cosineTheta / glm::pi<float>();
    return 1.0f;
}
}   // namespace raytracer