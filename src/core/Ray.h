#pragma once

#include <iostream>
#include <limits>

#include <glm/vec3.hpp>

namespace raytracer
{
/// @class Ray
/// @brief Ray Declaration
class Ray
{
public:
    /// @brief Default Ray constructor.
    Ray()
        : m_origin()
        , m_direction()
        , m_tMin(0.0f)
        , m_tMax(std::numeric_limits<float>::max()) {}

    /// @brief Ray constructor
    /// @param origin ray origin
    /// @param direction ray direction
    /// @param time time of the ray
    /// @param tMin minimum t-value to count as a hit
    /// @param tMax maximum t-value to count as a hit
    Ray(const glm::vec3 &origin,
        const glm::vec3 &direction,
        float tMin = 0.0f,
        float tMax = std::numeric_limits<float>::max())
            : m_origin(origin)
            , m_direction(direction)
            , m_tMin(tMin)
            , m_tMax(tMax) {}

    /// @brief Ray copy constructor
    /// @param other the ray to copy
    Ray(const Ray &other) = default;

    /// @brief Ray move constructor
    /// @param other the ray to move
    Ray(Ray &&other) noexcept = default;
    
    /// @brief Ray destructor
    ~Ray() = default;

    /// @brief Get the ray origin
    /// @return the ray origin
    glm::vec3 origin() const noexcept { return m_origin; }

    /// @brief Get the ray direction
    /// @return the ray direction
    glm::vec3 direction() const noexcept { return m_direction; }

    /// @brief Get the minimum t-value to count as a hit
    /// @return the minimum t-value to count as a hit
    float tMin() const noexcept { return m_tMin; }

    /// @brief Get the maximum t-value to count as a hit
    /// @return the maximum t-value to count as a hit
    float tMax() const noexcept { return m_tMax; }

    /// @brief Check if a t-value is within the ray's bounds
    /// @param t the t-value to check
    /// @return true if the t-value is within the ray's bounds, false otherwise
    bool contains(const float t) const noexcept { return t >= m_tMin && t <= m_tMax; }

    /// @brief The ray parametric equation
    /// @param t the parameter along the ray
    /// @return the point at distance t from the ray origin
    glm::vec3 operator()(const float t) const noexcept { return m_origin + m_direction * t; }

    /// @brief Ray copy assignment operator
    /// @param other the ray to copy
    /// @return reference to this ray
    Ray &operator=(const Ray &other) = default;

    /// @brief Ray move assignment operator
    /// @param other the ray to move
    /// @return reference to this ray
    Ray &operator=(Ray &&other) noexcept = default;

    friend std::ostream &operator<<(std::ostream &os, const Ray &r)
    {
        os << "[o=" << r.m_origin.x << "," << r.m_origin.y << "," << r.m_origin.z
           << ", d=" << r.m_direction.x << "," << r.m_direction.y << "," << r.m_direction.z
           << ", tMin=" << r.m_tMin << ", " << "tMax=" << r.m_tMax << "]";
        return os;
    }

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
    float m_tMin;   // abs(near) / m_direction.z
    float m_tMax;   // abs(zFar) / m_direction.z
};
} // namespace raytracer