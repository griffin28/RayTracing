#include "AABB.h"
#include "Ray.h"

namespace raytracer
{
//----------------------------------------------------------------------------------
AxisAlignedBoundingBox::AxisAlignedBoundingBox()
    : m_pMin(glm::vec3(std::numeric_limits<float>::max()))
    , m_pMax(glm::vec3(std::numeric_limits<float>::lowest()))
    , m_padding(0.0001f)
{}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox::AxisAlignedBoundingBox(const glm::vec3 &p1,
                                               const glm::vec3 &p2,
                                               float padding)
    : m_pMin(glm::vec3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)))
    , m_pMax(glm::vec3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)))
    , m_padding(padding)
{
    padToMinExtent();
}

//----------------------------------------------------------------------------------
glm::vec3 AxisAlignedBoundingBox::corner(const int c) const
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

    return glm::vec3(x,y,z);
}

//----------------------------------------------------------------------------------
bool AxisAlignedBoundingBox::intersect(const Ray &ray) const
{
    glm::vec3 invRayDir = glm::vec3(1.0f / ray.direction().x,
                                    1.0f / ray.direction().y,
                                    1.0f / ray.direction().z);

    // Absolute distance to lower and upper box coordinates
    glm::vec3 tLower = (m_pMin - ray.origin()) * invRayDir;
    glm::vec3 tUpper = (m_pMax - ray.origin()) * invRayDir;

    // The four t-intervals (for x-/y-/z-slabs, and ray p(t))
    glm::vec4 tMins = glm::vec4(glm::min(tLower, tUpper), ray.tMin());
    glm::vec4 tMaxes = glm::vec4(glm::max(tLower, tUpper), ray.tMax());

    auto tBoxMin = std::max(std::max(std::max(tMins[0], tMins[1]), tMins[2]), tMins[3]);
    auto tBoxMax = std::min(std::min(std::min(tMaxes[0], tMaxes[1]), tMaxes[2]), tMaxes[3]);

    return tBoxMin <= tBoxMax;
}

//----------------------------------------------------------------------------------
float AxisAlignedBoundingBox::surfaceArea() const
{
    glm::vec3 diagonal = m_pMax - m_pMin;
    auto retVal = 2 * (diagonal.x * diagonal.y + diagonal.x * diagonal.z + diagonal.y * diagonal.z);
    return retVal;
}

//----------------------------------------------------------------------------------
float AxisAlignedBoundingBox::volume() const
{
    glm::vec3 diagonal = m_pMax - m_pMin;
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
                                                       const glm::vec3 &point)
{
    return AxisAlignedBoundingBox(glm::vec3(std::min(box.m_pMin.x, point.x),
                                            std::min(box.m_pMin.y, point.y),
                                            std::min(box.m_pMin.z, point.z)),
                                  glm::vec3(std::max(box.m_pMax.x, point.x),
                                            std::max(box.m_pMax.y, point.y),
                                            std::max(box.m_pMax.z, point.z)));
}

//----------------------------------------------------------------------------------
void AxisAlignedBoundingBox::padToMinExtent()
{
    auto padding = m_padding * 0.5f;

    if( (m_pMax.x - m_pMin.x) < m_padding)
    {
        m_pMin.x -= padding;
        m_pMax.x += padding;
    }

    if( (m_pMax.y - m_pMin.y) < m_padding)
    {
        m_pMin.y -= padding;
        m_pMax.y += padding;
    }

    if( (m_pMax.z - m_pMin.z) < m_padding)
    {
        m_pMin.z -= padding;
        m_pMax.z += padding;
    }
}

} // namespace raytracer