#include "Sphere.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
bool Sphere::hit(const Ray &ray, HitRecord &record) const
{
    auto l = m_center - ray.origin();
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
        auto outwardNormal = glm::normalize(record.point - m_center);
        record.setFaceNormal(ray, outwardNormal);
        record.material = m_material;
        return true;
    }

    return false;
}
} // namespace raytracer