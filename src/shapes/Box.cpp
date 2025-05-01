#include "Box.h"
#include "Quad.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
Box::Box(const glm::dvec3 &a, const glm::dvec3 &b, std::shared_ptr<Material> material)
        : m_point1(a)
        , m_point2(b)
        , m_material(material) {}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox Box::getBounds() const
{
    auto modelMatrix = this->getModelMatrix();

    auto a = modelMatrix * glm::dvec4(m_point1, 1.0);
    auto b = modelMatrix * glm::dvec4(m_point2, 1.0);
    auto bounds = AxisAlignedBoundingBox(a, b);

    return bounds;
}

//----------------------------------------------------------------------------------
glm::dvec3 Box::center() const
{
    auto bounds = this->getBounds();
    auto min = bounds.m_pMin;
    auto max = bounds.m_pMax;

    return 0.5 * (min + max);
}

//----------------------------------------------------------------------------------
std::vector<Quad> Box::getSides() const
{
    auto bounds = this->getBounds();
    auto min = bounds.m_pMin;
    auto max = bounds.m_pMax;

    auto center = 0.5 * (min + max);

    // Create the sides
    glm::dvec3 dx = glm::dvec3(max.x - min.x, 0, 0);
    glm::dvec3 dy = glm::dvec3(0, max.y - min.y, 0);
    glm::dvec3 dz = glm::dvec3(0, 0, max.z - min.z);

    std::vector<Quad> sides(6);
    sides[0] = Quad(glm::dvec3(min.x, min.y, max.z), dx, dy, m_material);     // front
    sides[1] = Quad(glm::dvec3(max.x, min.y, max.z), -dz, dy, m_material);    // right
    sides[2] = Quad(glm::dvec3(max.x, min.y, min.z), -dx, dy, m_material);    // back
    sides[3] = Quad(glm::dvec3(min.x, min.y, min.z), dz, dy, m_material);     // left
    sides[4] = Quad(glm::dvec3(min.x, max.y, max.z), dx, -dz, m_material);    // top
    sides[5] = Quad(glm::dvec3(min.x, min.y, min.z), dx, dz, m_material);     // bottom

    return sides;
}

//----------------------------------------------------------------------------------
bool Box::hit(const Ray &ray, HitRecord &record) const
{
    auto bounds = this->getBounds();

    if(bounds.intersect(ray))
    {
        HitRecord tempRecord;
        bool hitAnything = false;
        double closest = std::numeric_limits<double>::max();
        auto sides = this->getSides();

        for(const auto &side : sides)
        {
            if(side.hit(ray, tempRecord))
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