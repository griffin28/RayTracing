#pragma once

#include "Pdf.h"
#include "Utility.h"

namespace raytracer
{
/// @class SpherePdf
/// @brief A PDF that generates random directions towards a sphere.
class SpherePdf : public Pdf
{
public:
    SpherePdf() = default;

    /// @brief Evaluate the PDF for a given direction.
    /// @param direction the direction to evaluate the PDF for
    /// @return the value of the PDF for the given direction
    float value(const glm::vec3 &direction) const override
    {
        return 1 / (4 * glm::pi<float>());
    }

    /// @brief Generate a random direction based on the PDF.
    /// @return a random direction based on the PDF
    glm::vec3 generate() const override
    {
        auto randomVector = RaytracingUtility::randomVector(-1,1);
        return glm::normalize(randomVector);
    }
};
} // namespace raytracer