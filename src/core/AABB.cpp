#include "AABB.h"
#include "Ray.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
AxisAlignedBoundingBox::AxisAlignedBoundingBox()
    : m_pMin(glm::vec3(std::numeric_limits<double>::max()))
    , m_pMax(glm::vec3(std::numeric_limits<double>::lowest()))
    , m_padding(0.0001)
{}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::dvec3 &p1,
                                               const glm::dvec3 &p2,
                                               double padding)
    : m_pMin(glm::vec3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)))
    , m_pMax(glm::vec3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)))
    , m_padding(padding)
{}

//----------------------------------------------------------------------------------
glm::dvec3 AxisAlignedBoundingBox::corner(const int c) const
{
    if(c >= 8 || c < 0)
    {
        std::string msg = "Invalid corner index (";
        msg += c;
        msg += "). Expecting index between 0-7 inclusive.";
        throw std::runtime_error(msg);
    }

    auto x = (c & 1) ? m_pMax.x : m_pMin.x;
    auto y = (c & 2) ? m_pMax.y : m_pMin.y;
    auto z = (c & 4) ? m_pMax.z : m_pMin.z;

    return glm::dvec3(x,y,z);
}

//----------------------------------------------------------------------------------
bool AxisAlignedBoundingBox::intersect(const Ray &ray) const
{
    glm::dvec3 invRayDir = glm::dvec3(1.0 / ray.direction().x,
                                      1.0 / ray.direction().y,
                                      1.0 / ray.direction().z);

    // Absolute distance to lower and upper box coordinates
    glm::dvec3 tLower = ((m_pMin - m_padding) - ray.origin()) * invRayDir;
    glm::dvec3 tUpper = ((m_pMax + m_padding) - ray.origin()) * invRayDir;

    // The four t-intervals (for x-/y-/z-slabs, and ray p(t))
    glm::dvec4 tMins = glm::dvec4(glm::min(tLower, tUpper), ray.tMin());
    glm::dvec4 tMaxes = glm::dvec4(glm::max(tLower, tUpper), ray.tMax());

    auto tBoxMin = std::max(std::max(std::max(tMins[0], tMins[1]), tMins[2]), tMins[3]);
    auto tBoxMax = std::min(std::min(std::min(tMaxes[0], tMaxes[1]), tMaxes[2]), tMaxes[3]);

    return tBoxMin <= tBoxMax;
}

//----------------------------------------------------------------------------------
double AxisAlignedBoundingBox::surfaceArea() const
{
    glm::dvec3 diagonal = m_pMax - m_pMin;
    auto retVal = 2 * (diagonal.x * diagonal.y + diagonal.x * diagonal.z + diagonal.y * diagonal.z);
    return retVal;
}

//----------------------------------------------------------------------------------
double AxisAlignedBoundingBox::volume() const
{
    glm::dvec3 diagonal = m_pMax - m_pMin;
    auto retVal = diagonal.x * diagonal.y * diagonal.z;
    return retVal;
}

//----------------------------------------------------------------------------------
int AxisAlignedBoundingBox::maxExtent() const
{
    glm::vec3 diagonal = m_pMax - m_pMin;

    if(diagonal.x > diagonal.y && diagonal.x > diagonal.z)
    {
        return 0;
    }
    else if(diagonal.y > diagonal.z)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox AxisAlignedBoundingBox::combine(const AxisAlignedBoundingBox &box1,
                                                       const AxisAlignedBoundingBox &box2)
{
    return AxisAlignedBoundingBox(glm::vec3(std::min(box1.m_pMin.x, box2.m_pMin.x),
                                            std::min(box1.m_pMin.y, box2.m_pMin.y),
                                            std::min(box1.m_pMin.z, box2.m_pMin.z)),
                                  glm::vec3(std::max(box1.m_pMax.x, box2.m_pMax.x),
                                            std::max(box1.m_pMax.y, box2.m_pMax.y),
                                            std::max(box1.m_pMax.z, box2.m_pMax.z)));
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox AxisAlignedBoundingBox::combine(const AxisAlignedBoundingBox &box,
                                                       const glm::dvec3 &point)
{
    return AxisAlignedBoundingBox(glm::dvec3(std::min(box.m_pMin.x, point.x),
                                            std::min(box.m_pMin.y, point.y),
                                            std::min(box.m_pMin.z, point.z)),
                                  glm::dvec3(std::max(box.m_pMax.x, point.x),
                                            std::max(box.m_pMax.y, point.y),
                                            std::max(box.m_pMax.z, point.z)));
}
} // namespace raytracer