#include <Color.h>
#include <Ray.h>
#include <PerspectiveCamera.h>

#include <iostream>
#include <memory>

using Color3f = raytracer::Color3f;
using Ray = raytracer::Ray;
using PerspectiveCamera = raytracer::PerspectiveCamera;

bool hit_sphere(const glm::vec3 &center, const float radius, Ray * const ray)
{
    auto l = center - ray->origin();
    auto tca = glm::dot(l, ray->direction());
    if(tca < 0)
    {
        return false;
    }
    auto d2 = glm::dot(l, l) - tca * tca;
    if(d2 > radius * radius)
    {
        return false;
    }
    float thc = sqrt(radius * radius - d2);
    float t0 = tca - thc;
    float t1 = tca + thc;

    if(t0 > t1)
    {
        std::swap(t0, t1);
    }

    if(t0 < 0)
    {
        t0 = t1;
        if(t0 < 0)
        {
            return false;
        }
    }

    if(ray->tMin() < t0 && t0 < ray->tMax())
    {
        return true;
    }

    return false;
}

Color3f ray_color(Ray * const ray)
{
    if(hit_sphere(glm::vec3(0, 0, -50), 10, ray))
    {
        return Color3f(1, 0, 0);
    }

    float a = 0.5f * (ray->direction().y + 1.0f);
    return (1.0f - a) * Color3f(1.0f, 1.0f, 1.0f) + a * Color3f(0.5f, 0.7f, 1.0f);
}

int main()
{
    // Image
    const int image_width = 500;
    const int image_height = 256;

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
            auto pixel_color = ray_color(ray.get());
            raytracer::write_color3f(std::cout, pixel_color);
        }
    }

    std::clog << "\nDone.\n";
    return 0;
}