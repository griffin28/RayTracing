#include "Sphere.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
Sphere::Sphere()
    : m_center(0.0)
    , m_radius(0.0)
    , m_material(nullptr)
    , m_bounds() {}

//----------------------------------------------------------------------------------
Sphere::Sphere(const glm::dvec3 &center, const double radius, std::shared_ptr<Material> material)
        : m_center(center)
        , m_radius(radius)
        , m_material(material)
{
    m_bounds = AxisAlignedBoundingBox(m_center - glm::dvec3(m_radius),
                                      m_center + glm::dvec3(m_radius),
                                      0.01);
}

//----------------------------------------------------------------------------------
bool Sphere::hit(const Ray &ray, HitRecord &record) const
{
    if(m_bounds.intersect(ray))
    {
        auto sphereCenter = m_center;
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

        double thc = sqrt(m_radius * m_radius - d2);
        double t0 = tca - thc;
        double t1 = tca + thc;

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

        if(ray.tMin() < t0 && t0 < ray.tMax())
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
} // namespace raytracer