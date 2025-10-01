#include "Lambertian.h"
#include "SolidColorTexture.h"
#include "Hittable.h"
#include "OrthoNormalBasis.h"

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
bool Lambertian::scatter(const Ray &ray, const HitRecord &record, glm::vec3 &attenuation, Ray &scattered, float &pdf) const
{
    OrthoNormalBasis onb(record.normal);
    glm::vec3 scatterDirection = onb.localToWorld(RaytracingUtility::randomCosineDirection());

    scattered = Ray(record.point, glm::normalize(scatterDirection));
    attenuation = m_albedo->value(record.u, record.v, record.point);
    pdf = glm::dot(onb.w(), scattered.direction()) / glm::pi<float>();
    return true;
}

//----------------------------------------------------------------------------------
float Lambertian::scatteringPDF(const Ray &ray, const HitRecord &record, const Ray &scattered) const
{
    float cosineTheta = glm::dot(record.normal, glm::normalize(scattered.direction()));
    return (cosineTheta < 0) ? 0 : cosineTheta / glm::pi<float>();
}

}   // namespace raytracer