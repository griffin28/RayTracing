#include "EmissiveMaterial.h"
#include "SolidColorTexture.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
EmissiveMaterial::EmissiveMaterial(const Color3d &color, double intensity)
    : m_texture(std::make_shared<SolidColorTexture>(color))
    , m_intensity(intensity)
{
}

//----------------------------------------------------------------------------------
EmissiveMaterial::EmissiveMaterial(const std::shared_ptr<Texture> &texture, double intensity)
    : m_texture(texture)
    , m_intensity(intensity)
{
}

//----------------------------------------------------------------------------------
Color3d EmissiveMaterial::emitted(double u, double v, const glm::dvec3 &point) const
{
    return m_intensity * m_texture->value(u, v, point);
}

} // namespace raytracer
