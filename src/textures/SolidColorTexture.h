#ifndef INCLUDED_SOLID_COLOR_TEXTURE_H
#define INCLUDED_SOLID_COLOR_TEXTURE_H

#include "Texture.h"

namespace raytracer
{

/// @class SolidColorTexture
/// @brief Solid color texture
class SolidColorTexture : public Texture
{
public:
    /// @brief Constructor
    /// @param color the color of the texture
    SolidColorTexture(const Color3f &color) : m_color(color) {}

    /// @brief Constructor
    /// @param r red channel
    /// @param g green channel
    /// @param b blue channel
    SolidColorTexture(float r, float g, float b) : m_color(r, g, b) {}

    /// @brief Destructor
    ~SolidColorTexture() = default;

    /// @see Texture::value
    Color3f value(float u = 0.f, float v = 0.f, const glm::vec3 &p = glm::vec3(0.f, 0.f, 0.f)) const override
    {
        return m_color;
    }

private:
    Color3f m_color;
};
}

#endif