#include "Sphere.h"

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
    m_center = glm::vec3(modelMatrix * glm::vec4(m_center, 1.0f));
}

//----------------------------------------------------------------------------------
void Sphere::updateBounds()
{
    m_bounds = AxisAlignedBoundingBox(m_center - glm::vec3(m_radius),
                                      m_center + glm::vec3(m_radius),
                                      0.01f);
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
    Hittable::scale(scale);
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