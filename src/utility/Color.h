#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>

namespace raytracer
{
    using Color3 = glm::dvec3;
    using Color4 = glm::dvec4;

    /// @brief Write a single pixel's color to the output stream.
    /// @param out the output stream
    /// @param pixel_color the color of the pixel (r,g,b) ranging from 0.0 to 1.0
    void write_color3(std::ostream& out, Color3 pixel_color)
    {
        // Write the translated [0,255] value of each color component.
        out << static_cast<int>(255.999 * pixel_color.x) << ' '
            << static_cast<int>(255.999 * pixel_color.y) << ' '
            << static_cast<int>(255.999 * pixel_color.z) << '\n';
    }

    /// @brief write a single pixel's color to the output stream.
    /// @param out the output stream
    /// @param pixel_color the color of the pixel (r,g,b,a) ranging
    ///                    from 0.0 to 1.0
    void write_color4(std::ostream& out, Color4 pixel_color)
    {
        // Write the translated [0,255] value of each color component and alpha.
        out << static_cast<int>(255.999 * pixel_color.x) << ' '
            << static_cast<int>(255.999 * pixel_color.y) << ' '
            << static_cast<int>(255.999 * pixel_color.z) << ' '
            << static_cast<int>(255.999 * pixel_color.w) << '\n';
    }
} // namespace raytracer