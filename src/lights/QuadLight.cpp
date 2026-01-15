#include "QuadLight.h"
#include "Quad.h"
#include "Texture.h"
#include "EmissiveMaterial.h"

#include <glm/gtx/norm.hpp>

namespace raytracer
{
//----------------------------------------------------------------------------------
QuadLight::QuadLight(const std::shared_ptr<Quad> &quad,
                     const Color3f &color,
                     float intensity)
    : m_quad(quad)
    , m_material(std::make_shared<EmissiveMaterial>(color, intensity))
{
    m_quad->setMaterial(m_material);
}

//----------------------------------------------------------------------------------
QuadLight::QuadLight(const std::shared_ptr<Quad> &quad,
                     const std::shared_ptr<Texture> &texture,
                     float intensity)
    : m_quad(quad)
    , m_material(std::make_shared<EmissiveMaterial>(texture, intensity))
{
    m_quad->setMaterial(m_material);
}

//----------------------------------------------------------------------------------
bool QuadLight::hit(const Ray &ray, HitRecord &record) const
{
    return m_quad->hit(ray, record);
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox QuadLight::getBounds() const
{
    return m_quad->getBounds();
}

//----------------------------------------------------------------------------------
glm::vec3 QuadLight::center() const
{
    return m_quad->center();
}

//----------------------------------------------------------------------------------
glm::vec3 QuadLight::randomPointOnSurface() const
{
   return m_quad->randomPointOnSurface();
}

//----------------------------------------------------------------------------------
float QuadLight::getSurfaceArea() const
{
    return m_quad->getSurfaceArea();
}

//----------------------------------------------------------------------------------
float QuadLight::pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const
{
    return m_quad->pdfValue(origin, direction);
}

//----------------------------------------------------------------------------------
float QuadLight::getGeometricFactor(const glm::vec3 &origin, const glm::vec3 &direction) const
{
    HitRecord record;
    Ray ray(origin, direction);

    if(this->hit(ray, record))
    {
        auto dir = glm::normalize(record.point - origin);
        const float dist2 = glm::length2(record.point - origin);
        const float cosine = std::abs(glm::dot(dir, record.normal));

        return cosine / dist2;
    }

    return 0.0f;
}

//----------------------------------------------------------------------------------
glm::vec3 QuadLight::random(const glm::vec3 &origin) const
{
    return m_quad->random(origin);
}

} // namespace raytracer