#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

namespace raytracer
{
class HittableList : public Hittable
{
public:
    HittableList() = default;

    /// @brief constructor to create a hittable list with a single object.
    /// @param object the hittable object to add to the list
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    /// @brief Clears the hittable list.
    void clear() { m_objects.clear(); }

    /// @brief Adds a hittable object to the list.
    void add(std::shared_ptr<Hittable> object) { m_objects.push_back(object); }

    /// @brief Determines if the ray intersects any of the objects in the list.
    /// @param ray the ray to test for intersection
    /// @param record the hit record to store the intersection information
    bool hit(const Ray &ray, HitRecord &record) const override
    {
        HitRecord tempRecord;
        bool hitAnything = false;
        auto closestSoFar = std::numeric_limits<double>::infinity();

        for (const auto &object : m_objects)
        {
            if (object->hit(ray, tempRecord))
            {
                hitAnything = true;

                if (tempRecord.t < closestSoFar)
                {
                    closestSoFar = tempRecord.t;
                    record = tempRecord;
                }
            }
        }

        return hitAnything;
    }

private:
    std::vector<std::shared_ptr<Hittable>> m_objects;
};
}