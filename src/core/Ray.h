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
    Ray(const Ray &other)
        : m_origin(other.m_origin)
        , m_direction(other.m_direction)
        , m_tMin(other.m_tMin)
        , m_tMax(other.m_tMax) {}

    /// @brief Ray destructor
    ~Ray() = default;

    /// @brief Get the ray origin
    /// @return the ray origin
    glm::vec3 origin() const { return m_origin; }

    /// @brief Get the ray direction
    /// @return the ray direction
    glm::vec3 direction() const { return m_direction; }

    /// @brief Get the minimum t-value to count as a hit
    /// @return the minimum t-value to count as a hit
    float tMin() const { return m_tMin; }

    /// @brief Get the maximum t-value to count as a hit
    /// @return the maximum t-value to count as a hit
    float tMax() const { return m_tMax; }

    /// @brief Check if a t-value is within the ray's bounds
    /// @param t the t-value to check
    /// @return true if the t-value is within the ray's bounds, false otherwise
    bool contains(const float t) const { return t >= m_tMin && t <= m_tMax; }

    /// @brief The ray parametric equation
    /// @param t the distance between the ray origin and the returned point
    /// @return the point at distance t from the ray origin
    glm::vec3 operator()(const float t) const { return m_origin + m_direction * t; }

    /// @brief Ray assignment operator
    /// @param other the ray to assign to this
    /// @return this ray
    Ray &operator=(const Ray &other)
    {
        if (this != &other)
        {
            m_origin = other.m_origin;
            m_direction = other.m_direction;
            m_tMin = other.m_tMin;
            m_tMax = other.m_tMax;
        }
        return *this;
    }

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
    mutable float m_tMin;   // abs(near) / m_direction.z
    mutable float m_tMax;   // abs(zFar) / m_direction.z
};
} // namespace raytracer