#ifndef INCLUDED_IMAGE_LOADER_H
#define INCLUDED_IMAGE_LOADER_H

#include <string>

namespace raytracer
{
    /// @class ImageLoader
    /// @brief A class for loading images from files.
    ///
    /// This class uses the stb_image library to load images from files. The pixel data
    /// is stored in memory and can be accessed using the pixelData() method. The width()
    /// and height() methods return the dimensions of the image.
    class ImageLoader
    {
    public:
        /// @brief Default constructor.
        ImageLoader() = default;

        /// @brief Constructor that loads an image from a file
        /// @param filename the name of the image file to load
        ///
        /// This constructor loads an image from a file and stores the pixel data in memory.
        /// If the RAYTRACER_IMAGES environment variable is set, the path to the image file
        /// is relative to the directory specified by the environment variable. If the image
        /// was not loaded successfully, the pixel data will be nullptr and width() and height()
        /// will return 0.
        ImageLoader(const char* filename);

        /// @brief Destructor.
        ~ImageLoader();

        /// @brief Load an image from a file.
        /// @param filename the name of the image file to load
        /// @return true if the image was loaded successfully, false otherwise
        ///
        /// This method loads an image from a file and stores the pixel data in memory.
        /// If the RAYTRACER_IMAGES environment variable is set, the path to the image file
        /// is relative to the directory specified by the environment variable. If the image
        /// was not loaded successfully, the pixel data will be nullptr and width() and height()
        /// will return 0.
        bool load(const std::string& filename);

        /// @brief Get the pixel data for a specific pixel.
        /// @param x the x-coordinate of the pixel
        /// @param y the y-coordinate of the pixel
        /// @return a pointer to the pixel data or magenta if there is no image data
        ///
        /// This method returns a pointer to the pixel data for the specified pixel. The pixel
        /// data is stored as an array of unsigned char values in RGB order. The pixel data
        /// is stored in memory in row-major order, so the pixel at (x, y) is located at
        /// pixelData(y * width()*3 + x*3).
        const unsigned char* pixelData(int x, int y) const;

        //@{
        /// @brief Get the image width and height.
        /// @return the width and height of the image
        int width() const { return m_width; }
        int height() const { return m_height; }
        //@}

    private:
        const int m_bytesPerPixel = 3;
        int m_width = 0;
        int m_height = 0;
        int m_bytesPerScanline = 0;
        unsigned char *m_pixelData = nullptr;

        template <typename T>
        static T clamp(const T &value, const T &min, const T &max)
        {
            if (value < min)
            {
                return min;
            }
            if (value > max)
            {
                return max;
            }
            return value;
        }
    };
}
#endif