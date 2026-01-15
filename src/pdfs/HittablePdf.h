#pragma once

#include "Pdf.h"
#include "Hittable.h"

#include <memory>

namespace raytracer
{
/// @class HittablePdf
/// @brief A PDF that generates random directions towards a hittable object.
class HittablePdf : public Pdf
{
public:   
    HittablePdf()
        : m_hittable(nullptr)
        , m_origin(0.0f)
    {
    }

    /// @brief Constructor
    /// @param hittable the hittable object
    /// @param origin the origin point
    HittablePdf(const std::shared_ptr<Hittable> &hittable, const glm::vec3 &origin)
        : m_hittable(hittable)
        , m_origin(origin)
    {
    }

    /// @brief Evaluate the PDF for a given direction.
    /// @param direction the direction to evaluate the PDF for
    /// @return the PDF value
    float value(const glm::vec3 &direction) const override
    { 
        return m_hittable->pdfValue(m_origin, direction);
    }

    /// @brief Generate a random direction based on the PDF.
    /// @return a random direction based on the PDF
    glm::vec3 generate() const override
    {
        return m_hittable->random(m_origin);
    }

private:
    std::shared_ptr<Hittable> m_hittable;
    glm::vec3 m_origin;

};
} // namespace raytracer