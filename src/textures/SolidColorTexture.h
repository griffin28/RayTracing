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
    SolidColorTexture(const Color3d &color) : m_color(color) {}

    /// @brief Constructor
    /// @param r red channel
    /// @param g green channel
    /// @param b blue channel
    SolidColorTexture(double r, double g, double b) : m_color(r, g, b) {}

    /// @brief Destructor
    ~SolidColorTexture() = default;

    /// @see Texture::value
    Color3d value(double u = 0, double v = 0, const glm::dvec3 &p = glm::dvec3(0, 0, 0)) const override
    {
        return m_color;
    }

private:
    Color3d m_color;
};
}

#endif