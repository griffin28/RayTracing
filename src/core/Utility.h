#pragma once

#include <random>

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
} // namespace raytracer