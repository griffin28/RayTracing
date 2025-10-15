#pragma once

#include "OrthoNormalBasis.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace raytracer
{

/// @class Pdf
/// @brief Abstract base class for probability density functions (PDFs).
/// A PDF is used to sample directions for importance sampling in ray tracing.
class Pdf
{
public:
    Pdf() = default;
    virtual ~Pdf() = default;

    /// @brief Evaluate the PDF for a given direction.
    /// @param direction the direction to evaluate the PDF for
    /// @return the value of the PDF for the given direction
    virtual float value(const glm::vec3 &direction) const = 0;

    /// @brief Generate a random direction based on the PDF.
    /// @return a random direction based on the PDF
    virtual glm::vec3 generate() const = 0;
};
} // namespace raytracer