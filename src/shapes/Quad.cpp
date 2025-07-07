#include "Quad.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
Quad::Quad()
    : m_Q(0.0f, 0.0f, 0.0f)
    , m_u(1.0f, 0.0f, 0.0f)
    , m_v(0.0f, 1.0f, 0.0f)
    , m_material(nullptr) {}

//----------------------------------------------------------------------------------
Quad::Quad(const glm::vec3 &Q,
           const glm::vec3 &u,
           const glm::vec3 &v,
           std::shared_ptr<Material> material)
    : m_Q(Q)
    , m_u(u)
    , m_v(v)
    , m_material(material) {}

//----------------------------------------------------------------------------------
bool Quad::hit(const Ray &ray, HitRecord &record) const
{
    auto normal = glm::normalize(this->getNormal());
    auto denom = glm::dot(normal, ray.direction());

    // Check if the ray is parallel to the quad
    if (std::abs(denom) < 1e-8)
    {
        return false;
    }

    // Check if hit point is within the ray interval
    auto t = (this->getD() - glm::dot(normal, ray.origin())) / denom;

    if (!ray.contains(t))
    {
        return false;
    }

    auto intersectionPoint = ray(t);

    // Check if the hit point is within the quad
    auto Q = this->getQ();
    auto u = this->getU();
    auto v = this->getV();
    auto w = this->getW();

    auto p = intersectionPoint - Q;
    auto alpha = glm::dot(w, glm::cross(p, v));
    auto beta = glm::dot(w, glm::cross(u, p));

    if(alpha < 0.0f || alpha > 1.0f || beta < 0.0f || beta > 1.0f)
    {
        return false;
    }

    // Record the hit
    record.u = alpha;
    record.v = beta;
    record.t = t;
    record.point = intersectionPoint;
    record.material = m_material;
    record.normal = normal;
    record.setFaceNormal(ray, record.normal);

    return true;
}

//----------------------------------------------------------------------------------
glm::vec3 Quad::center() const
{
    auto Q = this->getQ();
    auto u = this->getU();
    auto v = this->getV();

    // Calculate the center of the quad in world space
    auto center = Q + 0.5f * u + 0.5f * v;
    return center;
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox Quad::getBounds() const
{
    auto Q = this->getQ();
    auto u = this->getU();
    auto v = this->getV();

    return AxisAlignedBoundingBox(Q, Q + u + v);
}

//----------------------------------------------------------------------------------
glm::vec3 Quad::getQ() const
{
    glm::vec3 worldQ = glm::vec3(this->getModelMatrix() * glm::vec4(m_Q, 1.0f));
    return worldQ;
}

//----------------------------------------------------------------------------------
glm::vec3 Quad::getU() const
{
    glm::vec3 worldU = glm::vec3(this->getModelMatrix() * glm::vec4(m_u, 1.0f));
    return worldU;
}

//----------------------------------------------------------------------------------
glm::vec3 Quad::getV() const
{
    glm::vec3 worldV = glm::vec3(this->getModelMatrix() * glm::vec4(m_v, 1.0f));
    return worldV;
}

//----------------------------------------------------------------------------------
glm::vec3 Quad::getNormal() const
{
    auto u = this->getU();
    auto v = this->getV();
    auto n = glm::cross(u, v);
    return n;
}

//----------------------------------------------------------------------------------
double Quad::getD() const
{
    auto normal = glm::normalize(this->getNormal());
    auto Q = this->getQ();
    auto D = glm::dot(normal, Q);
    return D;
}

//----------------------------------------------------------------------------------
glm::vec3 Quad::getW() const
{
    auto normal = this->getNormal();
    auto w = normal / glm::dot(normal, normal);
    return w;
}

} // namespace raytracer