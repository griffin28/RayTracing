#include "BVH.h"
#include "AABB.h"

#include <algorithm>
#include <stack>

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
    BVHNode()
        : m_left(nullptr)
        , m_right(nullptr)
        , m_bounds() {}

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

        bool hitLeft = false, hitRight = false;

        if(m_left != nullptr)
        {
            hitLeft = m_left->hit(ray, record);
        }

        if(m_right != nullptr)
        {
            hitRight = m_right->hit(ray, record);
        }

        return hitLeft || hitRight;
    }

    //----------------------------------------------------------------------------------
    glm::vec3 center() const override
    {
        return 0.5f * m_bounds.m_pMin + 0.5f * m_bounds.m_pMax;
    }

    /// @brief left child node
    std::shared_ptr<Hittable> m_left;
    /// @brief right child node
    std::shared_ptr<Hittable> m_right;
    /// @brief bounds of the node
    AxisAlignedBoundingBox m_bounds;
};

//----------------------------------------------------------------------------------
BVH::BVH()
    : m_root(nullptr)
    , m_sceneObjects()
{
}

//----------------------------------------------------------------------------------
void BVH::build()
{
    if(m_sceneObjects.empty())
    {
        return;
    }

    std::clog << "Building BVH..." << std::endl;
    m_root = std::make_shared<BVHNode>();

    this->build(m_root, m_sceneObjects, 0, m_sceneObjects.size());

    // Print world bounds
    auto worldBounds = m_root->m_bounds;
    std::clog << "World Bounds" << std::endl;
    std::clog << "pMin: [" << worldBounds.m_pMin[0] << " , " << worldBounds.m_pMin[1] << " , " << worldBounds.m_pMin[2] << "]\n"
                 "pMax: [" << worldBounds.m_pMax[0] << " , " << worldBounds.m_pMax[1] << " , " << worldBounds.m_pMax[2] << "]\n";
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
void BVH::build(std::shared_ptr<BVHNode> node, const std::vector<std::shared_ptr<Hittable>> &sceneObjects, std::size_t start, std::size_t end)
{
    auto objects = sceneObjects; // Create a modifiable copy of the scene objects
    std::size_t objectSpan = end - start;

    if(objectSpan <= 0)
    {
        return;
    }

    // Compute bounds of all primitives in this node
    AxisAlignedBoundingBox bounds;

    if(objectSpan == 1)
    {
        bounds = objects[start]->getBounds();
    }
    else
    {
        for(std::size_t i = start; i < end; i++)
        {
            bounds = AxisAlignedBoundingBox::combine(bounds, objects[i]->getBounds());
        }
    }

    node->m_bounds = bounds;
    const int splitAxis = bounds.maxExtent();

    if(objectSpan == 1)
    {
        node->m_left = node->m_right = objects[start];
    }
    else if(objectSpan == 2)
    {
        if(objects[start]->center()[splitAxis] < objects[start + 1]->center()[splitAxis])
        {
            node->m_left = objects[start];
            node->m_right = objects[start + 1];
        }
        else
        {
            node->m_left = objects[start + 1];
            node->m_right = objects[start];
        }
    }
    else
    {
        // Sort the primitives along the longest axis of the centroid
        std::sort(objects.begin() + start, objects.begin() + end, [splitAxis](const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b)
                  {
                        return a->center()[splitAxis] < b->center()[splitAxis];
                  });

        // Recursively build the left and right subtrees
        std::size_t mid = (start + end) / 2;
        node->m_left = std::make_shared<BVHNode>();
        build(std::static_pointer_cast<BVHNode>(node->m_left), objects, start, mid);
        node->m_right = std::make_shared<BVHNode>();
        build(std::static_pointer_cast<BVHNode>(node->m_right), objects, mid, end);
    }
}
} // namespace raytracer