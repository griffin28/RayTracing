#pragma once

#include "Pdf.h"
#include "Utility.h"

namespace raytracer
{
/// @class CosinePdf
/// @brief A PDF that generates a random direction on the hemisphere with a cosine-weighted
///        distribution.
class CosinePdf : public Pdf
{
public:
    CosinePdf(const glm::vec3 &w)
        : m_onb(w)
    {
    }

    /// @brief Evaluate the PDF for a given direction.
    /// @param direction the direction to evaluate the PDF for
    /// @return the value of the PDF for the given direction
    float value(const glm::vec3 &direction) const override
    {
        const float cosineTheta = glm::dot(glm::normalize(direction), m_onb.w());
        return std::max(cosineTheta / glm::pi<float>(), 0.0f);
    }

    /// @brief Generate a random direction based on the PDF.
    /// @return a random direction based on the PDF
    glm::vec3 generate() const override
    {
        return m_onb.transform(RaytracingUtility::randomCosineDirection());
    }
private:
    OrthoNormalBasis m_onb;
};
} // namespace raytracer