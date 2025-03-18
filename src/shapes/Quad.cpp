#include "Quad.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
Quad::Quad(const glm::dvec3 &Q,
           const glm::dvec3 &u,
           const glm::dvec3 &v,
           std::shared_ptr<Material> material)
    : m_Q(Q)
    , m_u(u)
    , m_v(v)
    , m_material(material)
{
    auto n = glm::cross(m_u, m_v);
    m_normal = glm::normalize(n);
    m_D = glm::dot(m_normal, m_Q);
    m_w = n / glm::dot(n, n);

    this->setBoundingBox();
}

//----------------------------------------------------------------------------------
bool Quad::hit(const Ray &ray, HitRecord &record) const
{
    auto denom = glm::dot(m_normal, ray.direction());

    // Check if the ray is parallel to the quad
    if (std::abs(denom) < 1e-8)
    {
        return false;
    }

    // Check if hit point is within the ray interval
    auto t = (m_D - glm::dot(m_normal, ray.origin())) / denom;

    if (!ray.contains(t))
    {
        return false;
    }

    auto intersectionPoint = ray(t);

    // Check if the hit point is within the quad
    auto p = intersectionPoint - m_Q;
    auto alpha = glm::dot(m_w, glm::cross(p, m_v));
    auto beta = glm::dot(m_w, glm::cross(m_u, p));

    if(alpha < 0.0 || alpha > 1.0 || beta < 0.0 || beta > 1.0)
    {
        return false;
    }

    // Record the hit
    record.u = alpha;
    record.v = beta;
    record.t = t;
    record.point = intersectionPoint;
    record.material = m_material;
    record.normal = m_normal;
    record.setFaceNormal(ray, record.normal);

    return true;
}

//----------------------------------------------------------------------------------
void Quad::setBoundingBox()
{
    m_bounds = AxisAlignedBoundingBox(m_Q, m_Q + m_u + m_v);
}
} // namespace raytracer