#include "SphereLight.h"
#include "Texture.h"
#include "EmissiveMaterial.h"

#include <glm/gtx/norm.hpp>

namespace raytracer
{
//----------------------------------------------------------------------------------
SphereLight::SphereLight(const std::shared_ptr<Sphere> &quad,
                         const Color3f &color,
                         float intensity)
    : m_sphere(quad)
    , m_material(std::make_shared<EmissiveMaterial>(color, intensity))
{
    m_sphere->setMaterial(m_material);
}

//----------------------------------------------------------------------------------
SphereLight::SphereLight(const std::shared_ptr<Sphere> &quad,
                     const std::shared_ptr<Texture> &texture,
                     float intensity)
    : m_sphere(quad)
    , m_material(std::make_shared<EmissiveMaterial>(texture, intensity))
{
    m_sphere->setMaterial(m_material);
}

//----------------------------------------------------------------------------------
bool SphereLight::hit(const Ray &ray, HitRecord &record) const
{
    return m_sphere->hit(ray, record);
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox SphereLight::getBounds() const
{
    return m_sphere->getBounds();
}

//----------------------------------------------------------------------------------
glm::vec3 SphereLight::center() const
{
    return m_sphere->center();
}

//----------------------------------------------------------------------------------
glm::vec3 SphereLight::randomPointOnSurface() const
{
    return m_sphere->randomPointOnSurface();
}

//----------------------------------------------------------------------------------
float SphereLight::getGeometricFactor(const glm::vec3 &origin, const glm::vec3 &direction) const
{
    HitRecord record;
    Ray ray(origin, direction);

    if(this->hit(ray, record))
    {
        const float distanceSquared = glm::length2(record.point - origin);
        auto dir = glm::normalize(record.point - origin);
        return std::abs(-glm::dot(dir, record.normal)) / distanceSquared;
    }

    return 0.0f;
}

//----------------------------------------------------------------------------------
float SphereLight::pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const
{
    return m_sphere->pdfValue(origin, direction);
}

//----------------------------------------------------------------------------------
glm::vec3 SphereLight::random(const glm::vec3 &origin) const
{
    return m_sphere->random(origin);
}

} // namespace raytracer