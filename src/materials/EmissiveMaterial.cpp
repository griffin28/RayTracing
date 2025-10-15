#include "EmissiveMaterial.h"
#include "SolidColorTexture.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
EmissiveMaterial::EmissiveMaterial(const Color3f &color, float intensity)
    : m_texture(std::make_shared<SolidColorTexture>(color))
    , m_intensity(intensity)
{
}

//----------------------------------------------------------------------------------
EmissiveMaterial::EmissiveMaterial(const std::shared_ptr<Texture> &texture, float intensity)
    : m_texture(texture)
    , m_intensity(intensity)
{
}

//----------------------------------------------------------------------------------
Color3f EmissiveMaterial::emitted(const HitRecord &record) const
{
    if(!record.frontFace)
    {
        return Color3f(0,0,0);
    }

    return m_intensity * m_texture->value(record.u, record.v, record.point);
}

} // namespace raytracer
