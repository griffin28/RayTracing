#include <color.h>

#include <iostream>

int main()
{
    // Image
    const int image_width = 256;
    const int image_height = 256;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j=0; j < image_height; ++j)
    {
        std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
        for(int i=0; i < image_width; ++i)
        {
            auto r = static_cast<float>(i) / (image_width-1);
            auto g = static_cast<float>(j) / (image_height-1);
            auto b = 0.25f;

            auto pixel_color = raytracer::Color3f(r,g,b);
            raytracer::write_color3f(std::cout, pixel_color);
        }
    }

    std::clog << "\nDone.\n";
    return 0;
}