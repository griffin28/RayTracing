#pragma once

#include "Ray.h"
#include "Material.h"

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
        glm::vec3 point;
        glm::vec3 normal;
        std::shared_ptr<Material> material;
        float t;
        bool frontFace;

        /// @brief Sets the normal and front face flag based on the ray and outward normal.
        /// @param ray the ray that intersected the object
        /// @param outwardNormal the normal at the point of intersection
        void setFaceNormal(const Ray& ray, const glm::vec3& outwardNormal)
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
    };
} // namespace raytracer
