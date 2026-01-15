#ifndef INCLUDED_BVH_H
#define INCLUDED_BVH_H

#include "Hittable.h"

#include <vector>
#include <memory>

namespace raytracer
{
class AxisAlignedBoundingBox;
class Ray;
class BVHNode;

/// @class BVH
/// @brief Bounding Volume Hierarchy
///
/// Bounding volume hierarchy (BVH) is an approach for ray intersection acceleration based on
/// primitive subdivision, where the primitives are partitioned into a hierarchy of disjoint sets.
/// Primitives are stored in the leaves and each node stores a bounding box of the primitives
/// in the nodes beneath it. Thus, as a ray traverses through the tree, any time it doesn't
/// intersect a node's bounds, the subtree beneath that node can be skipped.
///
/// This implementation uses equal counts to partition primitives into equally sized subsets
/// when building the tree.
class BVH : public Hittable
{
public:
    /// @brief Default constructor
    // BVH(const std::vector<std::shared_ptr<Hittable>>);
    BVH();

    /// @brief Destructor
    ~BVH();

    /// @brief Build the BVH tree
    void build();

    /// @brief add a hittable object to the list.
    void add(std::shared_ptr<Hittable> object) { m_sceneObjects.push_back(object); }

    /// @brief Clear the hittable list
    void clear() { m_sceneObjects.clear(); }

    /// @see Hittable::getBounds
    AxisAlignedBoundingBox getBounds() const override;

    /// @see Hittable::hit
    bool hit(const Ray& ray, HitRecord& record) const override;

    /// @see Hittable::center
    virtual glm::vec3 center() const override;

    /// @brief Get a random point on a light source in the scene
    /// @param point the point on a random light source in the scene
    /// @return true if a point was found, false otherwise
    bool randomPointOnLight(glm::vec3 &point) const;

    /// @brief Get the light sources in the scene
    /// @return a vector of shared pointers to the light sources in the scene
    std::vector<std::shared_ptr<Hittable>> getLightSources() const;

private:
    std::unique_ptr<BVHNode> m_root;
    std::vector<std::shared_ptr<Hittable>> m_sceneObjects;
};
} // namespace raytracer

#endif