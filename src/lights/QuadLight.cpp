#include "QuadLight.h"
#include "Quad.h"
#include "Texture.h"
#include "EmissiveMaterial.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
QuadLight::QuadLight(const std::shared_ptr<Quad> &quad,
                     const Color3d &color,
                     double intensity)
    : m_quad(quad)
    , m_material(std::make_shared<EmissiveMaterial>(color, intensity))
{
    m_quad->setMaterial(m_material);
}

//----------------------------------------------------------------------------------
QuadLight::QuadLight(const std::shared_ptr<Quad> &quad,
                     const std::shared_ptr<Texture> &texture,
                     double intensity)
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
glm::dvec3 QuadLight::center() const
{
    return m_quad->center();
}
} // namespace raytracer