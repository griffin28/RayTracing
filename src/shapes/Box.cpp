#include "Box.h"
#include "Quad.h"


namespace raytracer
{
//----------------------------------------------------------------------------------
Box::Box(const glm::vec3 &p1, const glm::vec3 &p2, std::shared_ptr<Material> material)
        : m_point1(glm::vec3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)))
        , m_point2(glm::vec3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)))
        , m_material(material)
{
    this->createSides();
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox Box::getBounds() const
{
    return AxisAlignedBoundingBox(getWorldPoint1(), getWorldPoint2(), 0.000001f);
    // AxisAlignedBoundingBox bb;
    // for(const auto &side : m_sides)
    // {
    //     bb = AxisAlignedBoundingBox::combine(bb, side.getBounds());
    // }
    // return bb;
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

    auto point1 = this->getWorldPoint1();
    auto point2 = this->getWorldPoint2();

    auto min = glm::vec3(std::min(point1.x, point2.x), std::min(point1.y, point2.y), std::min(point1.z, point2.z));
    auto max = glm::vec3(std::max(point1.x, point2.x), std::max(point1.y, point2.y), std::max(point1.z, point2.z));

    // Create the sides
    glm::vec3 dx = glm::vec3(max.x - min.x, 0, 0);
    glm::vec3 dy = glm::vec3(0, max.y - min.y, 0);
    glm::vec3 dz = glm::vec3(0, 0, max.z - min.z);

    m_sides.push_back(Quad(glm::vec3(min.x, min.y, max.z), dx, dy, m_material));     // front
    m_sides.push_back(Quad(glm::vec3(max.x, min.y, min.z), dy, dz, m_material));    // right
    m_sides.push_back(Quad(glm::vec3(min.x, min.y, min.z), dx, dy, m_material));    // back
    m_sides.push_back(Quad(glm::vec3(min.x, min.y, min.z), dy, dz, m_material));     // left
    m_sides.push_back(Quad(glm::vec3(max.x, max.y, max.z), -dx, -dz, m_material));    // top
    m_sides.push_back(Quad(glm::vec3(min.x, min.y, min.z), dx, dz, m_material));     // bottom
}

//----------------------------------------------------------------------------------
bool Box::hit(const Ray &ray, HitRecord &record) const
{
    // Check if ray hits the bounding box
    // auto bounds = this->getBounds();
    
    // if(!bounds.intersect(ray))
    // {
    //     return false;
    // }
    
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

//----------------------------------------------------------------------------------
glm::vec3 Box::randomPointOnSurface(float &surfaceArea) const
{
    // Pick random side and get random point on that side
    if(m_sides.empty())
    {
        surfaceArea = 0.0f;
        return glm::vec3(0.0f);
    }

    // Generate a random index for the side
    int sideIndex = RaytracingUtility::randomInt(0, static_cast<int>(m_sides.size()) - 1);
    const auto &side = m_sides[sideIndex];
    auto randomPoint = side.randomPointOnSurface(surfaceArea);
    surfaceArea *= static_cast<float>(m_sides.size());
    return randomPoint;
}

//----------------------------------------------------------------------------------
float Box::pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const
{
    Ray ray(origin, direction);
    HitRecord record;

    if(this->hit(ray, record))
    {
        float area = 0.0f;
        this->randomPointOnSurface(area);

        const float distanceSquared = record.t * record.t * glm::dot(direction, direction);
        const float cosine = std::fabs(glm::dot(direction, record.normal) / glm::length(direction));
        
        return distanceSquared / (cosine * area);
    }

    return 0.0f;
}

//----------------------------------------------------------------------------------
glm::vec3 Box::random(const glm::vec3 &origin) const
{
    float surfaceArea;
    glm::vec3 randomPoint = this->randomPointOnSurface(surfaceArea);
    return randomPoint - origin;
}

} // namespace raytracer