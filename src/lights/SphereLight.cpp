#include "SphereLight.h"
#include "Sphere.h"
#include "Texture.h"
#include "EmissiveMaterial.h"

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
} // namespace raytracer