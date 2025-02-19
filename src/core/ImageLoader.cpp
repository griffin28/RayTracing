#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

namespace raytracer
{
//----------------------------------------------------------------------------------
ImageLoader::ImageLoader(const char* filename)
{
    auto filenameStr = std::string(filename);
    auto imageDir = std::getenv("RAYTRACER_IMAGES");

    if(imageDir)
    {
        this->load(std::string(imageDir) + "/" + filenameStr);
    }
    else
    {
        this->load(filenameStr);
    }
}

//----------------------------------------------------------------------------------
ImageLoader::~ImageLoader()
{
    if (m_pixelData)
    {
        stbi_image_free(m_pixelData);
    }
}

//----------------------------------------------------------------------------------
bool ImageLoader::load(const std::string& filename)
{
    m_pixelData = stbi_load(filename.c_str(), &m_width, &m_height, nullptr, m_bytesPerPixel);

    if (!m_pixelData)
    {
        m_width = 0;
        m_height = 0;

        std::clog << "Failed to load image: " << filename << std::endl;
        return false;
    }

    m_bytesPerScanline = m_bytesPerPixel * m_width;
    return true;
}

//----------------------------------------------------------------------------------
const unsigned char* ImageLoader::pixelData(int x, int y) const
{
    static unsigned char magenta[] = {255, 0, 255};

    if (!m_pixelData)
    {
        return magenta;
    }

    x = this->clamp(x, 0, m_width-1);
    y = this->clamp(y, 0, m_height-1);

    return m_pixelData + (y * m_bytesPerScanline + x * m_bytesPerPixel);
}

} // namespace raytracer