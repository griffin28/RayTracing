#pragma once

#include <random>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

namespace raytracer
{
    /// @brief Generate a random float in the range [0,1).
    /// @return a random float in the range [0,1)
    inline float randomFloat()
    {
        static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
        static std::mt19937 generator;
        static std::random_device device;
        generator.seed(device());
        return distribution(generator);
    }

    /// @brief Generate a random float in the range [min,max).
    /// @param min the minimum value of the range
    /// @param max the maximum value of the range
    /// @return a random float in the range [min,max)
    inline float randomFloat(float min, float max)
    {
        return min + (max - min) * randomFloat();
    }

    /// @brief Generate a random integer in the range [min,max].
    /// @param min the minimum value of the range
    /// @param max the maximum value of the range
    /// @return a random integer in the range [min,max]
    inline int randomInt(int min, int max)
    {
        return static_cast<int>(randomFloat(min, max + 1));
    }

    /// @brief Clamp a value to the range [min,max].
    /// @param value the value to clamp
    /// @param min the minimum value of the range
    /// @param max the maximum value of the range
    /// @return the clamped value
    inline float clamp(float value, float min, float max)
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
    inline glm::vec3 gammaCorrect(const glm::vec3 &color)
    {
        return glm::vec3(std::sqrt(color.x), std::sqrt(color.y), std::sqrt(color.z));
    }

    /// @brief Generate a random vector in the range [0,1).
    /// @return a random vector in the range [0,1)
    inline glm::vec3 randomVector()
    {
        return glm::vec3(randomFloat(), randomFloat(), randomFloat());
    }

    /// @brief Generate a random vector in the range [min,max).
    /// @param min the minimum value of the range
    inline glm::vec3 randomVector(float min, float max)
    {
        return glm::vec3(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
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
            if (glm::length(p) >= 1)
            {
                continue;
            }
            return p;
        }
    }

    /// @brief Generate a random vector on the unit hemisphere.
    /// This function uses rejection sampling to generate a random vector on the unit hemisphere.
    /// @param normal the normal of the hemisphere
    /// @return a random vector on the unit hemisphere
    inline glm::vec3 randomOnHemisphere(const glm::vec3 &normal)
    {
        glm::vec3 onUnitSphere = glm::normalize(randomInUnitSphere());

        if (glm::dot(onUnitSphere, normal) > 0.0f)
        {
            return onUnitSphere;
        }
        return -onUnitSphere;
    }
} // namespace raytracer