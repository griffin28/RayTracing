#include "Quad.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
Quad::Quad()
    : m_Q(0.0f, 0.0f, 0.0f)
    , m_u(1.0f, 0.0f, 0.0f)
    , m_v(0.0f, 1.0f, 0.0f)
    , m_material(nullptr) 
{
    // Need to update n first to get the correct d and w
    this->updateN();
    this->updateW();
    this->updateD();
}

//----------------------------------------------------------------------------------
Quad::Quad(const glm::vec3 &Q,
           const glm::vec3 &u,
           const glm::vec3 &v,
           std::shared_ptr<Material> material)
    : m_Q(Q)
    , m_u(u)
    , m_v(v)
    , m_material(material) 
{
    // Need to update n first to get the correct d and w
    this->updateN();
    this->updateW();
    this->updateD();
}

//----------------------------------------------------------------------------------
bool Quad::hit(const Ray &ray, HitRecord &record) const
{
    auto normal = glm::normalize(m_n);
    auto denom = glm::dot(normal, ray.direction());

    // Check if the ray is parallel to the quad
    if (std::abs(denom) < 1e-8)
    {
        return false;
    }

    // Check if hit point is within the ray interval
    auto t = (m_D - glm::dot(normal, ray.origin())) / denom;

    if (!ray.contains(t))
    {
        return false;
    }
    
    auto intersectionPoint = ray(t);

    // Check if the hit point is within the quad
    auto p = intersectionPoint - m_Q;
    auto alpha = glm::dot(m_w, glm::cross(p, m_v));
    auto beta = glm::dot(m_w, glm::cross(m_u, p));

    if((alpha < 0.0f) || (alpha > 1.0f) || (beta < 0.0f) || (beta > 1.0f))
    {
        return false;
    }

    // Record the hit
    record.u = alpha;
    record.v = beta;
    record.t = t;
    record.point = intersectionPoint;
    record.material = m_material;
    record.setFaceNormal(ray, normal);

    return true;
}

//----------------------------------------------------------------------------------
glm::vec3 Quad::center() const
{
    // Calculate the center of the quad in world space
    auto center = m_Q + 0.5f * m_u + 0.5f * m_v;
    return center;
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox Quad::getBounds() const
{
    return AxisAlignedBoundingBox(m_Q, m_Q + m_u + m_v, 0.0001f);
}

//----------------------------------------------------------------------------------
void Quad::rotate(const float angle, const glm::vec3 &axis)
{
    Hittable::rotate(angle, axis);
    this->updateQ();
    this->updateU();
    this->updateV();
    this->updateN();
    this->updateW();
    this->updateD(); 
}

//----------------------------------------------------------------------------------
void Quad::translate(const glm::vec3 &translation)
{
    Hittable::translate(translation);
    this->updateQ();
    this->updateU();
    this->updateV();
    this->updateN();
    this->updateW();
    this->updateD();
}

//----------------------------------------------------------------------------------
void Quad::scale(const glm::vec3 &scale)
{
    Hittable::scale(scale);
    this->updateQ();
    this->updateU();
    this->updateV();
    this->updateN();
    this->updateW();
    this->updateD();
}

//----------------------------------------------------------------------------------
void Quad::updateQ()
{
    m_Q = glm::vec3(this->getModelMatrix() * glm::vec4(m_Q, 1.0f));
}

//----------------------------------------------------------------------------------
void Quad::updateU()
{
    m_u = glm::vec3(this->getModelMatrix() * glm::vec4(m_u, 1.0f));
}

//----------------------------------------------------------------------------------
void Quad::updateV()
{
    m_v = glm::vec3(this->getModelMatrix() * glm::vec4(m_v, 1.0f));
}

//----------------------------------------------------------------------------------
void Quad::updateN()
{
    m_n = glm::cross(m_u, m_v);
}

//----------------------------------------------------------------------------------
void Quad::updateD()
{
    auto normal = glm::normalize(m_n);
    m_D = glm::dot(normal, m_Q);
}

//----------------------------------------------------------------------------------
void Quad::updateW()
{
    m_w = m_n / glm::dot(m_n, m_n);
}

} // namespace raytracer