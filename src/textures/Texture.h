#ifndef INCLUDED_TEXTURE_H
#define INCLUDED_TEXTURE_H

#include "Utility.h"

namespace raytracer
{

/// @class Texture
/// @brief Abstract base class for texture
class Texture
{
public:
    /// @brief Default constructor
    virtual ~Texture() = default;

    /// @brief Get the color value of the texture
    /// @param u the u texture coordinate
    /// @param v the v texture coordinate
    /// @param p the 3D point
    /// @return the RGB color value of the texture
    virtual Color3f value(float u, float v, const glm::vec3 &p) const = 0;
};
}

#endif