#pragma once

#include "Ray.h"
#include "Material.h"
#include "AABB.h"

#include <glm/glm.hpp>

#include <memory>

namespace raytracer
{
    /// @struct HitRecord
    /// @brief Structure to hold the hit record information.
    ///
    /// The hit record contains the point of intersection, the normal
    /// at the point of intersection, and the distance from the ray origin
    /// to the point of intersection.
    struct HitRecord
    {
        glm::dvec3 point;
        glm::dvec3 normal;
        std::shared_ptr<Material> material;
        double t;
        double u;
        double v;
        bool frontFace;

        HitRecord()
            : point()
            , normal()
            , material(nullptr)
            , t(-1.0)
            , u(0.0)
            , v(0.0)
            , frontFace(false)
        {
        }

        /// @brief Sets the normal and front face flag based on the ray and outward normal.
        /// @param ray the ray that intersected the object
        /// @param outwardNormal the normal at the point of intersection
        void setFaceNormal(const Ray& ray, const glm::dvec3& outwardNormal)
        {
            frontFace = glm::dot(ray.direction(), outwardNormal) < 0;
            normal = frontFace ? outwardNormal : -outwardNormal;
        }
    };

    /// @class Hittable
    /// @brief Abstract class for all hittable objects in the scene.
    class Hittable
    {
    public:
        virtual ~Hittable() = default;

        /// @brief Determines if the ray intersects the object.
        /// @param ray the ray to test for intersection
        /// @param record the hit record
        /// @return true if the ray intersects the object, false otherwise
        virtual bool hit(const Ray& ray, HitRecord& record) const = 0;

        /// @brief Get the world space bounds for this object
        /// @return Bounding box for the object using world space coordinates
        virtual AxisAlignedBoundingBox getBounds() const = 0;

        /// @brief Get the center of the object
        /// @return  the center of the object
        virtual glm::dvec3 center() const = 0;
    };
} // namespace raytracer
