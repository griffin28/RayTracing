#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

namespace raytracer
{
class OrthoNormalBasis
{
public:
    /// @brief Default constructor creates an identity basis.
    /// The default basis is the identity basis, where the u, v, and w vectors are
    /// aligned with the x, y, and z axes, respectively.
    OrthoNormalBasis()
    {
        m_u = glm::vec3(1,0,0);
        m_v = glm::vec3(0,1,0);
        m_w = glm::vec3(0,0,1);
    }

    /// @brief Construct an orthonormal basis from a given normal vector.
    /// @param n the normal vector
    explicit
    OrthoNormalBasis(const glm::vec3 &n)
    {
        buildFromNormal(n);
    }

    ~OrthoNormalBasis() = default;  

    /// @brief Build an orthonormal basis from a given normal vector.
    /// @param n the normal vector
    void buildFromNormal(const glm::vec3 &n)
    {
        m_w = glm::normalize(n);
        glm::vec3 a = (std::fabs(m_w.x) > 0.9f) ? glm::vec3(0,1,0) : glm::vec3(1,0,0);
        m_v = glm::normalize(glm::cross(m_w, a));
        m_u = glm::cross(m_w, m_v);
    }

    /// @brief Transforms a vector from local space to world space.
    /// @param a the vector in local space
    /// @return the vector in world space
    glm::vec3 localToWorld(const glm::vec3 &a) const
    {
        return a.x * m_u + a.y * m_v + a.z * m_w;
    }   

    /// @brief Transforms a vector from world space to local space.
    /// @param a the vector in world space
    /// @return the vector in local space
    glm::vec3 worldToLocal(const glm::vec3 &a) const
    {
        return glm::vec3(glm::dot(a, m_u), glm::dot(a, m_v), glm::dot(a, m_w));
    }       

    const glm::vec3 &u() const { return m_u; }
    const glm::vec3 &v() const { return m_v; }
    const glm::vec3 &w() const { return m_w; }  

private:
    glm::vec3 m_u; // tangent
    glm::vec3 m_v; // bitangent
    glm::vec3 m_w; // normal
};  
} // namespace raytracer