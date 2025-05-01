#include "Quad.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
Quad::Quad()
    : m_Q(0.0, 0.0, 0.0)
    , m_u(1.0, 0.0, 0.0)
    , m_v(0.0, 1.0, 0.0)
    , m_material(nullptr) {}

//----------------------------------------------------------------------------------
Quad::Quad(const glm::dvec3 &Q,
           const glm::dvec3 &u,
           const glm::dvec3 &v,
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
    record.normal = normal;
    record.setFaceNormal(ray, record.normal);

    return true;
}

//----------------------------------------------------------------------------------
glm::dvec3 Quad::center() const
{
    auto Q = this->getQ();
    auto u = this->getU();
    auto v = this->getV();

    // Calculate the center of the quad in world space
    auto center = Q + 0.5 * u + 0.5 * v;
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
glm::dvec3 Quad::getQ() const
{
    glm::dvec3 worldQ = glm::dvec3(this->getModelMatrix() * glm::dvec4(m_Q, 1.0));
    return worldQ;
}

//----------------------------------------------------------------------------------
glm::dvec3 Quad::getU() const
{
    glm::dvec3 worldU = glm::dvec3(this->getModelMatrix() * glm::dvec4(m_u, 1.0));
    return worldU;
}

//----------------------------------------------------------------------------------
glm::dvec3 Quad::getV() const
{
    glm::dvec3 worldV = glm::dvec3(this->getModelMatrix() * glm::dvec4(m_v, 1.0));
    return worldV;
}

//----------------------------------------------------------------------------------
glm::dvec3 Quad::getNormal() const
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
glm::dvec3 Quad::getW() const
{
    auto normal = this->getNormal();
    auto w = normal / glm::dot(normal, normal);
    return w;
}

} // namespace raytracer