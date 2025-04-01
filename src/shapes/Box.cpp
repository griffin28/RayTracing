#include "Box.h"
#include "Quad.h"

namespace raytracer
{

//----------------------------------------------------------------------------------
Box::Box(const glm::dvec3 &a, const glm::dvec3 &b, std::shared_ptr<Material> material)
        : m_point1(a)
        , m_point2(b)
        , m_material(material)
{
    auto min = glm::dvec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    auto max = glm::dvec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));

    m_center = 0.5 * (min + max);
    m_bounds = AxisAlignedBoundingBox(min, max);

    // Create the sides
    glm::dvec3 dx = glm::dvec3(max.x - min.x, 0, 0);
    glm::dvec3 dy = glm::dvec3(0, max.y - min.y, 0);
    glm::dvec3 dz = glm::dvec3(0, 0, max.z - min.z);

    m_sides[0] = std::make_unique<Quad>(glm::dvec3(min.x, min.y, max.z), dx, dy, material);     // front
    m_sides[1] = std::make_unique<Quad>(glm::dvec3(max.x, min.y, max.z), -dz, dy, material);    // right
    m_sides[2] = std::make_unique<Quad>(glm::dvec3(max.x, min.y, min.z), -dx, dy, material);    // back
    m_sides[3] = std::make_unique<Quad>(glm::dvec3(min.x, min.y, min.z), dz, dy, material);     // left
    m_sides[4] = std::make_unique<Quad>(glm::dvec3(min.x, max.y, max.z), dx, -dz, material);    // top
    m_sides[5] = std::make_unique<Quad>(glm::dvec3(min.x, min.y, min.z), dx, dz, material);     // bottom
}

//----------------------------------------------------------------------------------
bool Box::hit(const Ray &ray, HitRecord &record) const
{
    if(m_bounds.intersect(ray))
    {
        HitRecord tempRecord;
        bool hitAnything = false;
        double closest = std::numeric_limits<double>::max();

        for(const auto &side : m_sides)
        {
            if(side->hit(ray, tempRecord))
            {
                hitAnything = true;

                if(tempRecord.t < closest)
                {
                    closest = tempRecord.t;
                    record = tempRecord;
                }
            }
        }

        return hitAnything;
    }

    return false;
}
} // namespace raytracer