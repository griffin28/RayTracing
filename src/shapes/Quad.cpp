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
    setBoundingBox();
}

//----------------------------------------------------------------------------------
bool Quad::hit(const Ray &ray, HitRecord &record) const
{
    // Calculate the normal of the quad
    // auto normal = glm::cross(m_u, m_v);
    // auto denom = glm::dot(normal, ray.direction());

    // // Check if the ray is parallel to the quad
    // if (std::abs(denom) < 1e-6)
    // {
    //     return false;
    // }

    // auto t = glm::dot(normal, m_Q - ray.origin()) / denom;

    // // Check if the intersection is behind the ray
    // if (t < 0)
    // {
    //     return false;
    // }

    // auto P = ray.at(t);
    // auto w = P - m_Q;

    // auto uu = glm::dot(m_u, m_u);
    // auto uv = glm::dot(m_u, m_v);
    // auto vv = glm::dot(m_v, m_v);
    // auto wu = glm::dot(w, m_u);
    // auto wv = glm::dot(w, m_v);
    // auto D = uv * uv - uu * vv;

    // auto s = (uv * wv - vv * wu) / D;
    // if (s < 0 || s > 1)
    // {
    //     return false;
    // }

    // auto t1 = (uv * wu - uu * wv) / D;
    // if (t1 < 0 || t1 > 1)
    // {
    //     return false;
    // }

    // record.t = t;
    // record.point = P;
    // record.material = m_material;
    // record.normal = glm::normalize(normal);
    // record.setFaceNormal(ray, record.normal);

    return true;
}

//----------------------------------------------------------------------------------
void Quad::setBoundingBox()
{
    auto diagonal1 = AxisAlignedBoundingBox(m_Q, m_Q + m_u + m_v);
    auto diagonal2 = AxisAlignedBoundingBox(m_Q + m_u, m_Q + m_v);
    m_bounds = AxisAlignedBoundingBox::combine(diagonal1, diagonal2);
}
} // namespace raytracer