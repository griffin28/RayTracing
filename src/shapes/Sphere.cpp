#include "Sphere.h"
#include "OrthoNormalBasis.h"

#include <glm/gtx/norm.hpp>

namespace raytracer
{
//----------------------------------------------------------------------------------
Sphere::Sphere()
    : m_center(0.0)
    , m_radius(0.0)
    , m_material(nullptr) {}

//----------------------------------------------------------------------------------
Sphere::Sphere(const glm::vec3 &center, const float radius, std::shared_ptr<Material> material)
        : m_center(center)
        , m_radius(radius)
        , m_material(material) 
{
    this->updateBounds();
}

//----------------------------------------------------------------------------------
void Sphere::updateCenter()
{
    auto modelMatrix = this->getModelMatrix();
    glm::vec4 newCenter = modelMatrix * glm::vec4(m_center, 1.0f);
    m_center = glm::vec3(newCenter.x / newCenter.w,
                         newCenter.y / newCenter.w,
                         newCenter.z / newCenter.w);
}

//----------------------------------------------------------------------------------
void Sphere::updateBounds()
{
    m_bounds = AxisAlignedBoundingBox(m_center - glm::vec3(m_radius),
                                      m_center + glm::vec3(m_radius),
                                      0.000001f);
}

//----------------------------------------------------------------------------------
void Sphere::translate(const glm::vec3 &translation)
{
    Hittable::translate(translation);
    this->updateCenter();
    this->updateBounds();
}

//----------------------------------------------------------------------------------
void Sphere::rotate(const float angle, const glm::vec3 &axis)
{
    Hittable::rotate(angle, axis);
    this->updateCenter();
    this->updateBounds();
}

//----------------------------------------------------------------------------------
void Sphere::scale(const glm::vec3 &scale)
{
    m_radius *= scale.x; // assuming uniform scaling
    this->updateCenter();
    this->updateBounds();
}

//----------------------------------------------------------------------------------
bool Sphere::hit(const Ray &ray, HitRecord &record) const
{
    auto bounds = this->getBounds();

    if(bounds.intersect(ray))
    {
        auto sphereCenter = this->center();
        auto l = sphereCenter - ray.origin();
        auto tca = glm::dot(l, ray.direction());

        if(tca < 0)
        {
            return false;
        }

        auto d2 = glm::dot(l, l) - tca * tca;

        if(d2 > m_radius * m_radius)
        {
            return false;
        }

        float thc = sqrt(m_radius * m_radius - d2);
        float t0 = tca - thc;
        float t1 = tca + thc;

        if(t0 > t1)
        {
            std::swap(t0, t1);
        }

        if(t0 < 0)
        {
            t0 = t1;
            if(t0 < 0)
            {
                return false;
            }
        }

        if(ray.contains(t0))
        {
            record.t = t0;
            record.point = ray(t0);
            auto outwardNormal = glm::normalize(record.point - sphereCenter);
            record.setFaceNormal(ray, outwardNormal);
            record.material = m_material;
            Sphere::getSphereUV(outwardNormal, record.u, record.v);
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------------
glm::vec3 Sphere::randomPointOnSurface() const
{
    // Generate two random numbers in the range [0, 1)
    float u = static_cast<float>(RaytracingUtility::randomDouble());
    float v = static_cast<float>(RaytracingUtility::randomDouble());

    // Generate a random point on the surface of the sphere
    float theta = 2.0f * glm::pi<float>() * u;
    float phi = glm::acos(1.0f - 2.0f * v);
    float x = m_radius * sin(phi) * cos(theta);
    float y = m_radius * sin(phi) * sin(theta);
    float z = m_radius * cos(phi);

    return m_center + glm::vec3(x, y, z);
}

//----------------------------------------------------------------------------------
float Sphere::pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const
{
    HitRecord record;
    Ray ray(origin, direction);

    if(this->hit(ray, record))
    {
        auto dist2 = glm::length2(m_center - origin);
        auto cosThetaMax = glm::sqrt(1.0f - m_radius * m_radius / dist2);
        auto solidAngle = 2.0f * glm::pi<float>() * (1.0f - cosThetaMax);

        return 1.0f / solidAngle;
    }

    return 0.0f;
}

//----------------------------------------------------------------------------------
glm::vec3 Sphere::random(const glm::vec3 &origin) const
{
    auto direction = m_center - origin;
    auto dist2 = glm::length2(direction);
    OrthoNormalBasis uvw(direction);
    return uvw.transform(this->randomToSphere(m_radius, dist2));
}

//----------------------------------------------------------------------------------
glm::vec3 Sphere::randomToSphere(const float radius, const float distanceSquared) const 
{
    float r1 = RaytracingUtility::randomDouble();
    float r2 = RaytracingUtility::randomDouble();
    float z = 1.0f + r2 * (glm::sqrt(1.0f - radius * radius / distanceSquared) - 1.0f);

    float phi = 2.0f * glm::pi<float>() * r1;
    float x = glm::cos(phi) * glm::sqrt(1.0f - z * z);
    float y = glm::sin(phi) * glm::sqrt(1.0f - z * z);
    
    return glm::vec3(x, y, z);
}

} // namespace raytracer