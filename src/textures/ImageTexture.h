#ifndef INCLUDED_IMAGE_TEXTURE_H
#define INCLUDED_IMAGE_TEXTURE_H

#include "Texture.h"
#include "ImageLoader.h"

#include <memory>

namespace raytracer
{
/// @class ImageTexture
/// @brief Image texture
class ImageTexture : public Texture
{
public:
    /// @brief Constructor
    /// @param filename the name of the image file to load
    ImageTexture(const char *filename)
        : m_image(std::make_shared<ImageLoader>(filename))
    {
    }

    /// @brief Constructor
    /// @param image the image loader object
    ImageTexture(std::shared_ptr<ImageLoader> image)
        : m_image(image)
    {
    }

    ImageTexture() = delete;

    /// @brief Destructor
    ~ImageTexture() = default;

    /// @see Texture::value
    Color3f value(float u, float v, const glm::vec3 &p) const override
    {
        // Clamp input texture coordinates to [0,1] x [1,0]
        u = RaytracingUtility::clamp(u, 0.f, 1.f);
        v = 1.f - RaytracingUtility::clamp(v, 0.f, 1.f); // Flip V to image coordinates

        int i = static_cast<int>(u * m_image->width());
        int j = static_cast<int>(v * m_image->height());
        auto pixel = m_image->pixelData(i, j);

        // Convert pixel color to [0,1]
        float r = pixel[0] / 255.f;
        float g = pixel[1] / 255.f;
        float b = pixel[2] / 255.f;

        return Color3f(r, g, b);
    }

private:
    std::shared_ptr<ImageLoader> m_image;
};
}
#endif