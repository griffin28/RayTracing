#include "PerspectiveCamera.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

#include "Utility.h"
#include "Ray.h"
#include "HittableList.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <memory>

using PerspectiveCamera = raytracer::PerspectiveCamera;
using Ray = raytracer::Ray;
using HittableList = raytracer::HittableList;
using HitRecord = raytracer::HitRecord;

glm::dvec3 rayColor(Ray * const ray, int depth, const HittableList &world)
{
    if(depth <= 0)
    {
        return glm::dvec3(0.0);
    }

    HitRecord record;

    if(world.hit(*ray, record))
    {
        Ray scattered;
        glm::dvec3 attenuation(1.0);

        if(record.material->scatter(*ray, record, attenuation, scattered))
        {
            return attenuation * rayColor(&scattered, depth-1, world);
            // return gammaCorrect(attenuation * rayColor(&scattered, depth-1, world));
        }

        return glm::dvec3(0.0);
        // return 0.5 * (record.normal + glm::dvec3(1.0));
    }

    return glm::dvec3(0.678, 0.847, 0.902);
}

int main()
{
    const int IMAGE_WIDTH = 400;
    const int IMAGE_HEIGHT = 200;

    // Materials
    auto materialGround = std::make_shared<raytracer::Lambertian>(glm::dvec3(0.0, 0.8, 0.0));
    auto materialCenter = std::make_shared<raytracer::Lambertian>(glm::dvec3(0.7, 0.3, 0.3));
    auto materialLeft = std::make_shared<raytracer::Dielectric>(1.5);
    auto materialRight = std::make_shared<raytracer::Metal>(glm::dvec3(0.8, 0.6, 0.2));

    // World
    raytracer::HittableList world;
    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0,0,-50), 9, materialCenter));
    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(20,0,-50), 9, materialLeft));
    // world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(20,0,-50), -8, materialLeft));
    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(-20,0,-50), 9, materialRight));
    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0,-550.5,-200), 550, materialGround));

    // // Camera
    PerspectiveCamera camera(400, 200);
    // // camera.dolly(30);
    // // camera.boom(-50);
    // // camera.tilt(-70);

    camera.setFocalPoint(glm::dvec3(0,0,-41));
    camera.setAperatureRadius(0.1);

    camera.render(world, 4);

    // std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    // for(int j=0; j < IMAGE_HEIGHT; ++j)
    // {
    //     std::clog << "\rScanlines remaining: " << IMAGE_HEIGHT - j << ' ' << std::flush;

    //     for(int i=0; i < IMAGE_WIDTH; ++i)
    //     {
    //         std::unique_ptr<Ray> ray(camera.generateRay(glm::dvec2(i, j)));
    //         auto pixelColor = rayColor(ray.get(), 10, world);
    //         camera.writeColor3(std::cout, pixelColor, 1);
    //     }
    // }

    // std::clog << "\nDone.\n";
    return 0;
}