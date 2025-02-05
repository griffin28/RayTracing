#ifndef INCLUDED_CHECKER_TEXTURE_H
#define INCLUDED_CHECKER_TEXTURE_H

#include "Texture.h"
#include "SolidColorTexture.h"

#include <memory>

namespace raytracer
{

/// @class CheckerTecture
/// @brief creates a checker texture
class CheckerTexture : public Texture
{
public:
    /// @brief Constructor
    /// @param even the even texture
    /// @param odd the odd texture
    /// @param scale the scale of the texture
    CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd, double scale)
        : m_even(even)
        , m_odd(odd)
        , m_scale(scale) {}

    /// @brief Constructor
    /// @param color1 the first color that will be used for the even texture
    /// @param color2 the second color that will be used for the odd texture
    /// @param scale the scale of the texture
    CheckerTexture(glm::dvec3 color1, glm::dvec3 color2, double scale)
        : m_even(std::make_shared<SolidColorTexture>(color1))
        , m_odd(std::make_shared<SolidColorTexture>(color2))
        , m_scale(scale) {}

    /// @brief Destructor
    ~CheckerTexture() = default;

    /// @see Texture::value
    glm::dvec3 value(double u, double v, const glm::dvec3 &p) const override
    {
        double scaleInv = 1 / m_scale;
        auto xInt = static_cast<int>(std::floor(scaleInv * p.x));
        auto yInt = static_cast<int>(std::floor(scaleInv * p.y));
        auto zInt = static_cast<int>(std::floor(scaleInv * p.z));

        if((xInt + yInt + zInt) % 2 == 0)
            return m_even->value(u, v, p);
        else
            return m_odd->value(u, v, p);
    }

private:
    std::shared_ptr<Texture> m_even;
    std::shared_ptr<Texture> m_odd;
    double m_scale;
};
}

#endif