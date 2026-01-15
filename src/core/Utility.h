#ifndef INCLUDED_RAYTRACING_UTILITY_H
#define INCLUDED_RAYTRACING_UTILITY_H

#include <iostream>
#include <random>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp> // glm::pi
#include <glm/gtx/norm.hpp>    // glm::length2

namespace raytracer
{
// bluish color = 400-480 nm
// green color = 480-560 nm
// yellowish color = 560-580 nm
// reddish color = 580-645 nm
// reddish color = 645-750 nm
using Color3d = glm::dvec3;
using Color3f = glm::vec3;
using Color3i = glm::ivec3;

class RaytracingUtility
{
public:
    RaytracingUtility() = delete;
    ~RaytracingUtility() = delete;
    // Human perceptible wavelengths are between 380 and 750 nanometers.
    // The human eye is most sensitive to green light, so we use a gamma of 2.2.
    // This is a standard value for gamma correction.
    // https://en.wikipedia.org/wiki/SRGB
    // https://en.wikipedia.org/wiki/Gamma_correction

    /// @brief Generate a random double in the range [0,1).
    /// @return a random double in the range [0,1)
    static double randomDouble()
    {
        thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
        thread_local std::mt19937 generator(std::random_device{}());  // seed once
        return distribution(generator);
    }

    /// @brief Generate a random double in the range [min,max).
    /// @param min the minimum value of the range
    /// @param max the maximum value of the range
    /// @return a random double in the range [min,max)
    static double randomDouble(const double min, const double max)
    {
        return min + (max - min) * randomDouble();
    }

    /// @brief Generate a random integer in the range [min,max].
    /// @param min the minimum value of the range
    /// @param max the maximum value of the range
    /// @return a random integer in the range [min,max]
    static int randomInt(int min, int max)
    {
        std::uniform_int_distribution<int> distribution(min, max);
        thread_local std::mt19937 generator(std::random_device{}());
        return distribution(generator);
    }

    /// @brief Generate a random integer in the range [0, max_int].
    /// @return a random integer in the range [0, max_int]
    static int randomInt()
    {
        return randomInt(0, std::numeric_limits<int>::max());
    }

    /// @brief Convert a linear color to gamma-corrected color.
    /// @param color the linear color
    /// @return the gamma-corrected color
    static Color3f gammaCorrect(const Color3f &color)
    {
        return Color3f(glm::sqrt(color.x), glm::sqrt(color.y), glm::sqrt(color.z));
    }

    /// @brief Generate a random vector in the range [0,1).
    /// @return a random vector in the range [0,1)
    static glm::vec3 randomVector()
    {
        return glm::vec3(static_cast<float>(randomDouble()), static_cast<float>(randomDouble()), static_cast<float>(randomDouble()));
    }
    
    /// @brief Generate a random vector in the range [min,max).
    /// @param min the minimum value of the range
    /// @param max the maximum value of the range
    /// @return a random vector in the range [min,max)
    static glm::vec3 randomVector(const float min, const float max)
    {
        return glm::vec3(static_cast<float>(randomDouble(min, max)), 
                         static_cast<float>(randomDouble(min, max)),
                         static_cast<float>(randomDouble(min, max)));
    }

    /// @brief Generate a random unit vector.
    /// @return a random unit vector
    static glm::vec3 randomUnitVector()
    {
        while (true)
        {
            auto randVec = randomVector(-1.f, 1.f);
            auto len2 = glm::length2(randVec);

            if(len2 > 1e-160 && len2 <= 1.0f)
            {
                return glm::normalize(randVec);
            }
        }
    }

    /// @brief Generate a random vector in the unit sphere.
    /// This function uses rejection sampling to generate a random vector in the unit sphere.
    /// The algorithm works by generating a random vector in the unit cube and rejecting it if it
    /// is not in the unit sphere. This process is repeated until a vector in the unit sphere is
    /// found.
    /// @param maxAttempts the maximum number of attempts to generate a random vector in the unit sphere
    /// @return a random vector in the unit sphere
    static glm::vec3 randomInUnitSphere(const int maxAttempts = 10000)
    {
        for (int i = 0; i < maxAttempts; i++)
        {
            glm::vec3 p = randomVector(-1, 1);

            if (glm::length(p) < 1)
            {
                return p;
            }
        }

        std::clog << "Failed to generate a random vector in the unit sphere after " << maxAttempts << " attempts" << std::endl;
        return glm::vec3(1.0f, 1.0f, 1.0f);
    }

    /// @brief Generate a random cosine-weighted direction.
    /// This function generates a random direction on the hemisphere with a cosine-weighted
    /// distribution. This is useful for importance sampling in diffuse reflection.
    /// @return a random cosine-weighted direction
    static glm::vec3 randomCosineDirection()
    {
        auto r1 = randomDouble();
        auto r2 = randomDouble();
        auto phi = 2 * glm::pi<double>() * r1;

        auto x = static_cast<float>(glm::cos(phi) * glm::sqrt(r2));
        auto y = static_cast<float>(glm::sin(phi) * glm::sqrt(r2));
        auto z = static_cast<float>(glm::sqrt(1 - r2));

        return glm::vec3(x, y, z);
    }

    /// @brief Generate a random vector in the unit disk.
    /// This function uses rejection sampling to generate a random vector in the unit disk.
    /// The algorithm works by generating a random vector in the unit square and rejecting it if it
    /// is not in the unit disk. This process is repeated until a vector in the unit disk is found.
    /// @param maxAttempts the maximum number of attempts to generate a random vector in the unit disk
    /// @return a random vector in the unit disk
    static glm::vec3 randomInUnitDisk(const int maxAttempts = 10000)
    {
        for (int i = 0; i < maxAttempts; i++)
        {
            glm::vec3 p(static_cast<float>(randomDouble(-1, 1)), static_cast<float>(randomDouble(-1, 1)), 0.0f);

            if (glm::length(p) < 1)
            {
                return p;
            }
        }

        std::clog << "Failed to generate a random vector in the unit disk after " << maxAttempts << " attempts" << std::endl;
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    /// @brief Generate a random vector on the unit hemisphere.
    /// This function uses rejection sampling to generate a random vector on the unit hemisphere.
    /// @param normal the normal of the hemisphere
    /// @return a random vector on the unit hemisphere
    static glm::vec3 randomOnHemisphere(const glm::vec3 &normal)
    {
        auto onUnitSphere = glm::normalize(randomInUnitSphere());

        if (glm::dot(onUnitSphere, normal) > 0.0f)
        {
            return onUnitSphere;
        }
        return -onUnitSphere;
    }
};
} // namespace raytracer

#endif