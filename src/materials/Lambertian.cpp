#include "Lambertian.h"
#include "SolidColorTexture.h"
#include "CosinePdf.h"
// #include "Hittable.h"

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
bool Lambertian::scatter(const Ray &ray, const HitRecord &record, ScatterRecord &scatterRecord) const
{
    scatterRecord.attenuation = m_albedo->value(record.u, record.v, record.point);
    scatterRecord.pdfPtr = std::make_shared<CosinePdf>(record.normal);
    scatterRecord.skipPdf = false;
    
    return true;
}

//----------------------------------------------------------------------------------
float Lambertian::scatteringPDF(const Ray &ray, const HitRecord &record, const Ray &scattered) const
{
    float cosineTheta = glm::dot(record.normal, glm::normalize(scattered.direction()));
    return (cosineTheta < 0) ? 0 : cosineTheta / glm::pi<float>();
}

}   // namespace raytracer