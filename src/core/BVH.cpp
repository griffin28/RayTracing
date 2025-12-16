#include "BVH.h"
#include "AABB.h"

#include <algorithm>
#include <iostream>

namespace raytracer
{
/// @class BVHNode
/// @brief A class to represent a node in the BVH tree.
///
/// The BVH node contains a left and right child node, and a bounding box
/// that encompasses all the objects in the node.
class BVHNode : public Hittable
{
public:
    BVHNode(std::vector<std::shared_ptr<Hittable>> &objects) 
        : BVHNode(objects, 0, objects.size()) {}
        
    BVHNode(std::vector<std::shared_ptr<Hittable>> &objects, size_t start, size_t end) 
    {
        size_t objectSpan = end - start;

        // Compute bounds of all primitives in this node
        AxisAlignedBoundingBox bounds;
        for(size_t i = start; i < end; i++)
        {
            bounds = AxisAlignedBoundingBox::combine(bounds, objects[i]->getBounds());
        }
        m_bounds = bounds;

        if(objectSpan == 1)
        {
            m_left = m_right = objects[start];
        }
        else if(objectSpan == 2)
        {
            if(objects[start]->getBounds().pMin()[bounds.maxExtent()] < objects[start + 1]->getBounds().pMin()[bounds.maxExtent()])
            {
                m_left = objects[start];
                m_right = objects[start + 1];
            }
            else
            {
                m_left = objects[start + 1];
                m_right = objects[start];
            }
        }
        else
        {
            // Sort the primitives along the longest axis of the centroid
            std::sort(objects.begin() + start, objects.begin() + end, [bounds](const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b)
                      {
                            return a->center()[bounds.maxExtent()] < b->center()[bounds.maxExtent()];
                      });

            // Recursively build the left and right subtrees
            size_t mid = (start + end) / 2;
            m_left = std::make_shared<BVHNode>(objects, start, mid);
            m_right = std::make_shared<BVHNode>(objects, mid, end);
        }
    }

    ~BVHNode() = default;

    //----------------------------------------------------------------------------------
    AxisAlignedBoundingBox getBounds() const override
    {
        return m_bounds;
    }

    //----------------------------------------------------------------------------------
    bool hit(const Ray &ray, HitRecord &record) const override
    {
        if(!m_bounds.intersect(ray))
        {
            return false;
        }

        const bool hitLeft = m_left->hit(ray, record);

        // Create narrowed ray for right child if left child was hit
        Ray rightRay = hitLeft ? Ray(ray.origin(), ray.direction(), ray.tMin(), record.t)
                               : ray;
        const bool hitRight = m_right->hit(rightRay, record);

        return hitLeft || hitRight;
    }

    //----------------------------------------------------------------------------------
    glm::vec3 center() const override
    {
        return 0.5f * (m_bounds.pMin() + m_bounds.pMax());
    }

    /// @brief left child node
    std::shared_ptr<Hittable> m_left;
    /// @brief right child node
    std::shared_ptr<Hittable> m_right;
    /// @brief bounds of the node
    AxisAlignedBoundingBox m_bounds;
};

//----------------------------------------------------------------------------------
BVH::BVH() : m_root(nullptr) {}

BVH::~BVH() {}

//----------------------------------------------------------------------------------
void BVH::build()
{
    if(m_sceneObjects.empty())
    {
        return;
    }

    std::clog << "Building BVH..." << std::endl;
    m_root.reset(new BVHNode(m_sceneObjects));

    // Print world bounds
    auto worldBounds = m_root->m_bounds;
    std::clog << "World Bounds" << std::endl;
    std::clog << "pMin: [" << worldBounds.pMin()[0] << " , " << worldBounds.pMin()[1] << " , " << worldBounds.pMin()[2] << "]\n"
                 "pMax: [" << worldBounds.pMax()[0] << " , " << worldBounds.pMax()[1] << " , " << worldBounds.pMax()[2] << "]\n";
}

//----------------------------------------------------------------------------------
AxisAlignedBoundingBox BVH::getBounds() const
{
    return m_root ? m_root->m_bounds : AxisAlignedBoundingBox();
}

//----------------------------------------------------------------------------------
glm::vec3 BVH::center() const
{
    return m_root ? m_root->center() : glm::vec3(0);
}

//----------------------------------------------------------------------------------
bool BVH::hit(const Ray& ray, HitRecord& record) const
{
    if(!m_root)
    {
        return false;
    }

    return m_root->hit(ray, record);
}

//----------------------------------------------------------------------------------
bool BVH::randomPointOnLight(glm::vec3 &point, float &surfaceArea) const
{
    if(m_sceneObjects.empty())
    {
        // throw std::runtime_error("No objects in the scene.");
        return false;
    }

    // Find all light sources in the scene
    std::vector<std::shared_ptr<Hittable>> lights;
    for(const auto &obj : m_sceneObjects)
    {
        if(obj->isLight())
        {
            lights.push_back(obj);
        }
    }

    if(lights.empty())
    {
        // throw std::runtime_error("No light sources found in the scene.");
        return false;
    }

    // Randomly select a light source and return a random point on it
    auto light = lights[RaytracingUtility::randomInt(0, lights.size() - 1)];
    point = light->randomPointOnSurface(surfaceArea);

    return true;
}

//----------------------------------------------------------------------------------
std::vector<std::shared_ptr<Hittable>> BVH::getLightSources() const
{
    std::vector<std::shared_ptr<Hittable>> lights;

    for(const auto &obj : m_sceneObjects)
    {
        if(obj->isLight())
        {
            lights.push_back(obj);
        }
    }

    return lights;
}

} // namespace raytracer