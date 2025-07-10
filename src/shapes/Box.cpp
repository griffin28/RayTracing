#include "Box.h"
#include "Quad.h"


namespace raytracer
{
//----------------------------------------------------------------------------------
Box::Box(const glm::vec3 &a, const glm::vec3 &b, std::shared_ptr<Material> material)
        : m_point1(a)
        , m_point2(b)
        , m_material(material) 
{
    this->createSides();
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox Box::getBounds() const
{
    return AxisAlignedBoundingBox(getWorldPoint1(), getWorldPoint2(), 0.0001f);
}

//----------------------------------------------------------------------------------
void Box::translate(const glm::vec3 &translation)
{
    Hittable::translate(translation);
    this->createSides();
}

//----------------------------------------------------------------------------------
void Box::rotate(const float angle, const glm::vec3 &axis)
{
    Hittable::rotate(angle, axis);
    this->createSides();
}

//----------------------------------------------------------------------------------
void Box::scale(const glm::vec3 &scale)
{
    Hittable::scale(scale);
    this->createSides();
}

//----------------------------------------------------------------------------------
glm::vec3 Box::getWorldPoint1() const
{
    auto modelMatrix = this->getModelMatrix();
    return glm::vec3(modelMatrix * glm::vec4(m_point1, 1.0f));
}

//----------------------------------------------------------------------------------
glm::vec3 Box::getWorldPoint2() const
{
    auto modelMatrix = this->getModelMatrix();
    return glm::vec3(modelMatrix * glm::vec4(m_point2, 1.0f));
}

//----------------------------------------------------------------------------------
glm::vec3 Box::center() const
{
    auto point1 = this->getWorldPoint1();
    auto point2 = this->getWorldPoint2();

    return 0.5f * (point1 + point2);
}

//----------------------------------------------------------------------------------
void Box::createSides()
{
    m_sides.clear(); 
    m_sides.reserve(6);
    
    // Get the world space points
    auto point1 = this->getWorldPoint1();
    auto point2 = this->getWorldPoint2();

    auto min = glm::vec3(std::min(point1.x, point2.x), std::min(point1.y, point2.y), std::min(point1.z, point2.z));
    auto max = glm::vec3(std::max(point1.x, point2.x), std::max(point1.y, point2.y), std::max(point1.z, point2.z));

    // Create the sides
    glm::vec3 dx = glm::vec3(max.x - min.x, 0, 0);
    glm::vec3 dy = glm::vec3(0, max.y - min.y, 0);
    glm::vec3 dz = glm::vec3(0, 0, max.z - min.z);

    m_sides.push_back(Quad(glm::vec3(min.x, min.y, max.z), dx, dy, m_material));     // front
    m_sides.push_back(Quad(glm::vec3(max.x, min.y, max.z), -dz, dy, m_material));    // right
    m_sides.push_back(Quad(glm::vec3(max.x, min.y, min.z), -dx, dy, m_material));    // back
    m_sides.push_back(Quad(glm::vec3(min.x, min.y, min.z), dz, dy, m_material));     // left
    m_sides.push_back(Quad(glm::vec3(min.x, max.y, max.z), dx, -dz, m_material));    // top
    m_sides.push_back(Quad(glm::vec3(min.x, min.y, min.z), dx, dz, m_material));     // bottom
}

//----------------------------------------------------------------------------------
bool Box::hit(const Ray &ray, HitRecord &record) const
{
    HitRecord tempRecord;
    bool hit = false;
    auto closestT = ray.tMax();
    
    // Check each side of the box for intersection
    for(const auto &side : m_sides)
    {
        if(side.hit(ray, tempRecord))
        {
            // If the hit is closer than the closest hit, update the record
            if(tempRecord.t < closestT)
            {
                closestT = tempRecord.t;
                record = tempRecord;
                hit = true;
            }
        }
    }   
    
    return hit;
}
} // namespace raytracer