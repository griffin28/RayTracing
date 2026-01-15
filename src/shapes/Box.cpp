#include "Box.h"
#include "Quad.h"

#include <glm/gtx/norm.hpp>

namespace raytracer
{
//----------------------------------------------------------------------------------
Box::Box(const glm::vec3 &a,
         const glm::vec3 &b,
         std::shared_ptr<Material> material)
        : m_points({glm::vec3(a.x, a.y, b.z),
                    glm::vec3(b.x, a.y, b.z),
                    glm::vec3(b.x, a.y, a.z),
                    a,
                    glm::vec3(a.x, b.y, b.z),
                    b,
                    glm::vec3(b.x, b.y, a.z),
                    glm::vec3(a.x, b.y, a.z)})
        , m_material(material)
{
    this->createSides();
}

//----------------------------------------------------------------------------------
Box::Box(std::vector<glm::vec3> points, std::shared_ptr<Material> material)
        : m_points(points)
        , m_material(material)
{
    this->createSides();
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox Box::getBounds() const
{
    auto worldPoints = this->getWorldPoints();
    glm::vec3 minPoint( std::numeric_limits<float>::max());
    glm::vec3 maxPoint(-std::numeric_limits<float>::max());

    for(const auto &point : worldPoints)
    {
        minPoint = glm::min(minPoint, point);
        maxPoint = glm::max(maxPoint, point);
    }

    AxisAlignedBoundingBox bb(minPoint, maxPoint);
    return bb;
}

//----------------------------------------------------------------------------------
void Box::translate(const glm::vec3 &translation)
{
    this->setModelMatrix(glm::translate(this->getModelMatrix(), translation));
    this->createSides();
}

//----------------------------------------------------------------------------------
void Box::rotate(const float angle, const glm::vec3 &axis)
{
    const auto c = this->center();
    const auto translationToOrigin = glm::translate(glm::mat4(1.0f), -c);
    const auto rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    const auto translationBack = glm::translate(glm::mat4(1.0f), c);

    this->setModelMatrix(translationBack * rotationMatrix * translationToOrigin * this->getModelMatrix());
    this->createSides();
}

//----------------------------------------------------------------------------------
void Box::scale(const glm::vec3 &scale)
{
    const auto c = this->center();
    const auto translationToOrigin = glm::translate(glm::mat4(1.0f), -c);
    const auto scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    const auto translationBack = glm::translate(glm::mat4(1.0f), c);

    this->setModelMatrix(translationBack * scaleMatrix * translationToOrigin * this->getModelMatrix());
    this->createSides();
}

//----------------------------------------------------------------------------------
std::vector<glm::vec3> Box::getWorldPoints() const
{
    std::vector<glm::vec3> worldPoints;
    worldPoints.reserve(m_points.size());

    auto modelMatrix = this->getModelMatrix();

    for(const auto &point : m_points)
    {
        auto homogeneousPoint = glm::vec4(point, 1.0f);
        auto coord = modelMatrix * homogeneousPoint;
        coord /= coord.w;
        worldPoints.push_back(glm::vec3(coord));
    }

    return worldPoints;
}

//----------------------------------------------------------------------------------
glm::vec3 Box::center() const
{
    auto worldPoints = this->getWorldPoints();
    glm::vec3 minPoint( std::numeric_limits<float>::max());
    glm::vec3 maxPoint(-std::numeric_limits<float>::max());

    for(const auto &point : worldPoints)
    {
        minPoint = glm::min(minPoint, point);
        maxPoint = glm::max(maxPoint, point);
    }

    return 0.5f * (minPoint + maxPoint);
}

//----------------------------------------------------------------------------------
void Box::createSides()
{
    m_sides.clear(); 
    m_sides.reserve(6);

    if(m_points.size() < 8)
    {
        return;
    }

    auto worldPoints = this->getWorldPoints();

    // Bottom vertices counter-clockwise
    auto p0 = worldPoints[0];
    auto p1 = worldPoints[1];
    auto p2 = worldPoints[2];
    auto p3 = worldPoints[3];
    // Top vertices counter-clockwise
    auto p4 = worldPoints[4];
    auto p5 = worldPoints[5];
    auto p6 = worldPoints[6];
    auto p7 = worldPoints[7];

    // Create the sides using the corner points
    m_sides.push_back(std::make_shared<Quad>(p0, p1 - p0, p4 - p0, m_material)); // Front
    m_sides.push_back(std::make_shared<Quad>(p1, p2 - p1, p5 - p1, m_material)); // Right
    m_sides.push_back(std::make_shared<Quad>(p2, p3 - p2, p6 - p2, m_material)); // Back
    m_sides.push_back(std::make_shared<Quad>(p3, p0 - p3, p7 - p3, m_material)); // Left
    m_sides.push_back(std::make_shared<Quad>(p4, p5 - p4, p7 - p4, m_material)); // Top
    m_sides.push_back(std::make_shared<Quad>(p0, p1 - p0, p3 - p0, m_material)); // Bottom
}

//----------------------------------------------------------------------------------
bool Box::hit(const Ray &ray, HitRecord &record) const
{
    // Check if ray hits the bounding box
    auto bounds = this->getBounds();
    
    if(!bounds.intersect(ray))
    {
        return false;
    }
    
    HitRecord tempRecord;
    bool hit = false;
    auto closestT = ray.tMax();
    
    // Check each side of the box for intersection
    for(const auto &side : m_sides)
    {
        if(side->hit(ray, tempRecord))
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
glm::vec3 Box::randomPointOnSurface() const
{
    // Pick random side and get random point on that side
    if(m_sides.empty())
    {
        return glm::vec3(0.0f);
    }

    // Generate a random index for the side
    int sideIndex = RaytracingUtility::randomInt(0, static_cast<int>(m_sides.size()) - 1);
    const auto &side = m_sides[sideIndex];
    auto randomPoint = side->randomPointOnSurface();
    return randomPoint;
}

//----------------------------------------------------------------------------------
float Box::getSurfaceArea() const
{
    float totalArea = 0.0f;
    for(const auto &side : m_sides)
    {
        totalArea += side->getSurfaceArea();
    }
    return totalArea;
}

//----------------------------------------------------------------------------------
float Box::pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const
{
    Ray ray(origin, direction);
    HitRecord record;

    if(this->hit(ray, record))
    {
        auto dir = glm::normalize(record.point - origin);
        const float dist2 = glm::length2(record.point - origin);
        const float cosine = std::abs(glm::dot(dir, record.normal));
        const float surfaceArea = this->getSurfaceArea();
        // return dist2 / (cosine * surfaceArea);
        // const float distanceSquared = record.t * record.t * glm::dot(direction, direction);
        // const float cosine = std::fabs(glm::dot(direction, record.normal) / glm::length(direction));
        
        return cosine / (dist2 * surfaceArea);
    }

    return 0.0f;
}

//----------------------------------------------------------------------------------
glm::vec3 Box::random(const glm::vec3 &origin) const
{
    glm::vec3 randomPoint = this->randomPointOnSurface();
    return randomPoint - origin;
}

} // namespace raytracer