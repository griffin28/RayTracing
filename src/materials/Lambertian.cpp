#include "Lambertian.h"
#include "SolidColorTexture.h"
#include "Hittable.h"
#include "CosinePdf.h"

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
    attenuation = m_albedo->value(record.u, record.v, record.point);

    CosinePdf cosinePdf(record.normal);
    glm::vec3 direction = cosinePdf.generate();
    scattered = Ray(record.point, direction);
    pdf = cosinePdf.value(scattered.direction());
    
    return true;
}

//----------------------------------------------------------------------------------
float Lambertian::scatteringPDF(const Ray &ray, const HitRecord &record, const Ray &scattered) const
{
    // float cosineTheta = glm::dot(record.normal, glm::normalize(scattered.direction()));
    // return (cosineTheta < 0) ? 0 : cosineTheta / glm::pi<float>();
    return 1 / (4 * glm::pi<float>());
}

}   // namespace raytracer