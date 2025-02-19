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
    CheckerTexture(const std::shared_ptr<Texture> &even, const std::shared_ptr<Texture> &odd, double scale)
        : m_even(even)
        , m_odd(odd)
        , m_scale(scale) {}

    /// @brief Constructor
    /// @param evenColor the first color that will be used for the even texture
    /// @param oddColor the second color that will be used for the odd texture
    /// @param scale the scale of the texture
    CheckerTexture(const Color3d &evenColor, const Color3d &oddColor, double scale)
        : m_even(std::make_shared<SolidColorTexture>(evenColor))
        , m_odd(std::make_shared<SolidColorTexture>(oddColor))
        , m_scale(scale) {}

    CheckerTexture() = delete;

    /// @brief Destructor
    ~CheckerTexture() = default;

    /// @see Texture::value
    Color3d value(double u, double v, const glm::dvec3 &p) const override
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