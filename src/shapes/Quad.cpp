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
    return AxisAlignedBoundingBox(m_Q, m_Q + m_u + m_v, 0.0000001f);
}

//----------------------------------------------------------------------------------
void Quad::rotate(const float angle, const glm::vec3 &axis)
{
    // TODO: Add corner points and rotate around center
}

//----------------------------------------------------------------------------------
void Quad::translate(const glm::vec3 &translation)
{
    // TODO
}

//----------------------------------------------------------------------------------
void Quad::scale(const glm::vec3 &scale)
{
    // TODO
}

//----------------------------------------------------------------------------------
void Quad::updateQ()
{
    // TODO
}

//----------------------------------------------------------------------------------
void Quad::updateU()
{
    // TODO
}

//----------------------------------------------------------------------------------
void Quad::updateV()
{
    // TODO
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

//----------------------------------------------------------------------------------
glm::vec3 Quad::randomPointOnSurface(float &surfaceArea) const
{
    // Generate two random numbers in the range [0, 1)
    float u = static_cast<float>(RaytracingUtility::randomDouble());
    float v = static_cast<float>(RaytracingUtility::randomDouble());

    // Compute the random point on the quad
    glm::vec3 randomPoint = m_Q + u * m_u + v * m_v;

    surfaceArea = glm::length(m_n);

    return randomPoint;
}

//----------------------------------------------------------------------------------
float Quad::pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const
{
    HitRecord record;
    Ray ray(origin, direction);

    if(this->hit(ray, record))
    {
        float area = glm::length(m_n);
        float distanceSquared = record.t * record.t * glm::dot(direction, direction);
        float cosine = std::abs(glm::dot(direction, record.normal) / glm::length(direction));

        return distanceSquared / (cosine * area);
    }
    
    return 0.0f;
}

//----------------------------------------------------------------------------------
glm::vec3 Quad::random(const glm::vec3 &origin) const
{
    float surfaceArea;
    glm::vec3 randomPoint = this->randomPointOnSurface(surfaceArea);
    return randomPoint - origin;
}

} // namespace raytracer