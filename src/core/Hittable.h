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
        glm::vec3 point;
        glm::vec3 normal;
        std::shared_ptr<Material> material;
        float t;
        float u;
        float v;
        bool frontFace;

        HitRecord()
            : point()
            , normal()
            , material(nullptr)
            , t(-1.0f)
            , u(0.0f)
            , v(0.0f)
            , frontFace(false)
        {
        }

        /// @brief Sets the normal and front face flag based on the ray and outward normal.
        /// @param ray the ray that intersected the object
        /// @param outwardNormal the normal at the point of intersection
        void setFaceNormal(const Ray& ray, const glm::vec3& outwardNormal)
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
        virtual glm::vec3 center() const = 0;

        /// @brief translate the object in world space
        /// @param translation the coordinates, in world space, of a translation vector
        /// @note This function modifies the model matrix of the object.
        virtual void translate(const glm::vec3& translation)
        {
            m_modelMatrix = glm::translate(m_modelMatrix, translation);
        }

        /// @brief rotate the object in world space
        /// @param angle the angle to rotate in degrees
        /// @param axis rotation axis, recommended to be normalized
        /// @note This function modifies the model matrix of the object.
        virtual void rotate(const float angle, const glm::vec3& axis)
        {
            m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(angle), axis);
        }

        /// @brief scale the object in world space
        /// @param scale ratio of scaling for each axis
        /// @note This function modifies the model matrix of the object.
        virtual void scale(const glm::vec3& scale)
        {
            m_modelMatrix = glm::scale(m_modelMatrix, scale);
        }

        /// @brief Determine if the object is a light source
        /// @return true if the object is a light source, false otherwise
        virtual bool isLight() const { return false; }

        /// @brief Get a random point on the surface of the object
        /// @param surfaceArea the surface area of the object
        /// @return a random point on the surface of the object
        virtual glm::vec3 randomPointOnSurface(float &surfaceArea) const
        {
            surfaceArea = 0.0f;
            return glm::vec3(0.0f);
        }

        /// @brief Get the PDF value for the given origin and direction
        /// @param origin the origin of the ray
        /// @param direction the direction of the ray
        /// @return the PDF value
        virtual float pdfValue(const glm::vec3 &origin, const glm::vec3 &direction) const
        {
            return 0.0f;
        }

        /// @brief Get a random direction from the given origin
        /// @param origin the origin of the ray
        /// @return a random direction based on the PDF
        virtual glm::vec3 random(const glm::vec3 &origin) const
        {
            return glm::vec3(1.0f, 0.0f, 0.0f);
        }

        /// @brief get the model matrix for the object
        /// @return the model matrix for the object
        glm::mat4 getModelMatrix() const
        {
            return m_modelMatrix;
        }

        /// @brief set the model matrix for the object
        /// @param modelMatrix the model matrix to set
        /// @note This function modifies the model matrix of the object.
        void setModelMatrix(const glm::mat4& modelMatrix)
        {
            m_modelMatrix = modelMatrix;
        }

    private:
        glm::mat4 m_modelMatrix;
    };
} // namespace raytracer
