#include "Color.h"
#include "Ray.h"
#include "PerspectiveCamera.h"
#include "HittableList.h"
#include "Sphere.h"

#include <iostream>
#include <memory>

using Color3f = raytracer::Color3f;
using Ray = raytracer::Ray;
using PerspectiveCamera = raytracer::PerspectiveCamera;

Color3f ray_color(Ray * const ray, const raytracer::HittableList &world)
{
    raytracer::HitRecord record;
    if(world.hit(*ray, record))
    {
        return 0.5f * Color3f(record.normal.x + 1, record.normal.y + 1, record.normal.z + 1);
    }

    float a = 0.5f * (ray->direction().y + 1.0f);
    return (1.0f - a) * Color3f(1.0f, 1.0f, 1.0f) + a * Color3f(0.5f, 0.7f, 1.0f);
}

int main()
{
    // Image
    const int image_width = 500;
    const int image_height = 256;

    // World
    raytracer::HittableList world;
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0,0,-50), 10));
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0,-150.5,-1), 1000));

    // Camera
    PerspectiveCamera camera(image_width, image_height);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j=0; j < image_height; ++j)
    {
        std::clog << "\rScanlines remaining: " << image_height - j << ' ' << std::flush;
        for(int i=0; i < image_width; ++i)
        {
            std::unique_ptr<Ray> ray(camera.generateRay(glm::vec2(i, j)));
            auto pixel_color = ray_color(ray.get(), world);
            raytracer::write_color3f(std::cout, pixel_color);
        }
    }

    std::clog << "\nDone.\n";
    return 0;
}