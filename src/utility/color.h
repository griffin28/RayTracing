#pragma once

#include <glm/glm.hpp>

#include <iostream>

using Color3f = glm::vec3;
using Color4f = glm::vec4;

/// @brief Write a single pixel's color to the output stream.
/// @param out the output stream
/// @param pixel_color the color of the pixel (r,g,b)
void write_color3f(std::ostream& out, Color3f pixel_color)
{
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x) << ' '
        << static_cast<int>(255.999 * pixel_color.y) << ' '
        << static_cast<int>(255.999 * pixel_color.z) << '\n';
}

/// @brief write a single pixel's color to the output stream.
/// @param out the output stream
/// @param pixel_color the color of the pixel (r,g,b,a)
void write_color4f(std::ostream& out, Color4f pixel_color)
{
    // Write the translated [0,255] value of each color component and alpha.
    out << static_cast<int>(255.999 * pixel_color.x) << ' '
        << static_cast<int>(255.999 * pixel_color.y) << ' '
        << static_cast<int>(255.999 * pixel_color.z) << ' '
        << static_cast<int>(255.999 * pixel_color.w) << '\n';
}