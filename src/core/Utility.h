#ifndef INCLUDED_RAYTRACER_UTILITY_H
#define INCLUDED_RAYTRACER_UTILITY_H

#include <iostream>
#include <random>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>

namespace raytracer
{
    // Human perceptible wavelengths are between 380 and 750 nanometers.
    // The human eye is most sensitive to green light, so we use a gamma of 2.2.
    // This is a standard value for gamma correction.
    // https://en.wikipedia.org/wiki/SRGB
    // https://en.wikipedia.org/wiki/Gamma_correction\

    // bluish color = 400-480 nm
    // green color = 480-560 nm
    // yellowish color = 560-580 nm
    // reddish color = 580-645 nm
    // reddish color = 645-750 nm
    using Color3d = glm::dvec3;
    using Color3f = glm::vec3;
    using Color3i = glm::ivec3;

    /// @brief Generate a random double in the range [0,1).
    /// @return a random double in the range [0,1)
    inline double randomDouble()
    {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        static std::random_device device;
        generator.seed(device());
        return distribution(generator);
    }

    /// @brief Generate a random double in the range [min,max).
    /// @param min the minimum value of the range
    /// @param max the maximum value of the range
    /// @return a random double in the range [min,max)
    inline double randomDouble(const double min, const double max)
    {
        return min + (max - min) * randomDouble();
    }

    /// @brief Generate a random integer in the range [min,max].
    /// @param min the minimum value of the range
    /// @param max the maximum value of the range
    /// @return a random integer in the range [min,max]
    inline int randomInt(int min, int max)
    {
        return static_cast<int>(randomDouble(min, max + 1));
    }

    /// @brief Clamp a value to the range [min,max].
    /// @param value the value to clamp
    /// @param min the minimum value of the range
    /// @param max the maximum value of the range
    /// @return the clamped value
    inline double clamp(double value, double min, double max)
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

    /// @brief Clamp a value to the range [min,max].
    /// @param value the value to clamp
    /// @param min the minimum value of the range
    /// @param max the maximum value of the range
    /// @return the clamped value
    inline float clampf(float value, float min, float max)
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

    /// @brief Convert a linear color to gamma-corrected color.
    /// @param color the linear color
    /// @return the gamma-corrected color
    inline Color3f gammaCorrect(Color3d &color)
    {
        return Color3f(std::sqrt(color.x), std::sqrt(color.y), std::sqrt(color.z));
    }

    /// @brief Generate a random vector in the range [0,1).
    /// @return a random vector in the range [0,1)
    inline glm::vec3 randomVector()
    {
        return glm::vec3(static_cast<float>(randomDouble()), static_cast<float>(randomDouble()), static_cast<float>(randomDouble()));
    }

    /// @brief Generate a random vector in the range [min,max).
    /// @param min the minimum value of the range
    inline glm::vec3 randomVector(const float min, const float max)
    {
        return glm::vec3(static_cast<float>(randomDouble(min, max)), 
                         static_cast<float>(randomDouble(min, max)),
                         static_cast<float>(randomDouble(min, max)));
    }

    /// @brief Generate a random vector in the unit sphere.
    /// This function uses rejection sampling to generate a random vector in the unit sphere.
    /// The algorithm works by generating a random vector in the unit cube and rejecting it if it
    /// is not in the unit sphere. This process is repeated until a vector in the unit sphere is
    /// found.
    /// @return a random vector in the unit sphere
    inline glm::vec3 randomInUnitSphere()
    {
        while (true)
        {
            glm::vec3 p = randomVector(-1, 1);

            if (glm::length(p) < 1)
            {
                return p;
            }
        }
    }

    /// @brief Generate a random vector in the unit disk.
    /// This function uses rejection sampling to generate a random vector in the unit disk.
    /// The algorithm works by generating a random vector in the unit square and rejecting it if it
    /// is not in the unit disk. This process is repeated until a vector in the unit disk is found.
    /// @return a random vector in the unit disk
    inline glm::vec3 randomInUnitDisk()
    {
        while (true)
        {
            glm::vec3 p(static_cast<float>(randomDouble(-1, 1)), static_cast<float>(randomDouble(-1, 1)), 0.0f);

            if (glm::length(p) < 1)
            {
                return p;
            }
        }
    }

    /// @brief Generate a random vector on the unit hemisphere.
    /// This function uses rejection sampling to generate a random vector on the unit hemisphere.
    /// @param normal the normal of the hemisphere
    /// @return a random vector on the unit hemisphere
    inline glm::vec3 randomOnHemisphere(const glm::vec3 &normal)
    {
        auto onUnitSphere = glm::normalize(randomInUnitSphere());

        if (glm::dot(onUnitSphere, normal) > 0.0f)
        {
            return onUnitSphere;
        }
        return -onUnitSphere;
    }
} // namespace raytracer

#endif