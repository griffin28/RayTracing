#pragma once

#include "Pdf.h"
#include "CosinePdf.h"
#include "HittablePdf.h"
#include "Utility.h"

#include <memory>
#include <vector>



namespace raytracer
{
/// @class MixturePdf
/// @brief A PDF that combines multiple PDFs.
class MixturePdf : public Pdf
{
public:  
    /// @brief Constructor
    /// @param pdfs the list of PDFs to combine
    MixturePdf(const std::vector<std::shared_ptr<Pdf>> &pdfs)
        : m_pdfs(pdfs)
    {
    }

    /// @brief Evaluate the PDF for a given direction.
    /// @param direction direction to evaluate the PDF for
    /// @return the PDF value
    float value(const glm::vec3 &direction) const override
    {
        float pdfValue = 0.0f;
        float weight = 1.0f / static_cast<float>(m_pdfs.size());

        for(std::size_t i = 0; i < m_pdfs.size(); ++i)
        {
            pdfValue += m_pdfs[i]->value(direction) * weight;
        }

        return pdfValue;
    }

    /// @brief Generate a random direction based on the PDF.
    /// @return a random direction based on the PDF
    glm::vec3 generate() const override
    {
        int index = 0;
        
        if(m_pdfs.size() > 1)
        {
            index = RaytracingUtility::randomInt(0, static_cast<int>(m_pdfs.size()) - 1);
        }
        
        return m_pdfs[index]->generate();
    }

private:
    std::vector<std::shared_ptr<Pdf>> m_pdfs;
};
} // namespace raytracer