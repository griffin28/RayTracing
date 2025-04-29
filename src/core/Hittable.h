#pragma once

#include "Ray.h"
#include "Material.h"
#include "AABB.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

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

        /// @brief Assignment operator to copy another HitRecord.
        /// @param other the other HitRecord to copy from
        HitRecord& operator=(const HitRecord& other)
        {
            if (this != &other)
            {
                point = other.point;
                normal = other.normal;
                material = other.material;
                t = other.t;
                u = other.u;
                v = other.v;
                frontFace = other.frontFace;
            }

            return *this;
        }
    };

    /// @class Hittable
    /// @brief Abstract class for all hittable objects in the scene.
    class Hittable
    {
    public:
        /// @brief Default constructor
        Hittable(): m_modelMatrix(1.0) {}

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

        /// @brief translate the object in world space
        /// @param translation the translation vector
        /// @note This function modifies the model matrix of the object.
        virtual void translate(const glm::dvec3& translation)
        {
            m_modelMatrix = glm::translate(m_modelMatrix, translation);
        }

        /// @brief rotate the object in world space
        /// @param angle the angle to rotate in degrees
        /// @param axis the axis to rotate around
        /// @note This function modifies the model matrix of the object.
        /// @note The axis is in world space coordinates.
        virtual void rotate(const double angle, const glm::dvec3& axis)
        {
            m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(angle), axis);
        }

        /// @brief scale the object in world space
        /// @param scale the scale factor
        /// @note This function modifies the model matrix of the object.
        virtual void scale(const glm::dvec3& scale)
        {
            m_modelMatrix = glm::scale(m_modelMatrix, scale);
        }

        /// @brief get the model matrix for the object
        /// @return the model matrix for the object
        glm::dmat4 getModelMatrix() const
        {
            return m_modelMatrix;
        }

        /// @brief set the model matrix for the object
        /// @param modelMatrix the model matrix to set
        /// @note This function modifies the model matrix of the object.
        void setModelMatrix(const glm::dmat4& modelMatrix)
        {
            m_modelMatrix = modelMatrix;
        }

    private:
        glm::dmat4 m_modelMatrix;
    };
} // namespace raytracer
