#include "PerspectiveCamera.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
#include "Utility.h"
#include "Ray.h"
#include "BVH.h"
#include "CheckerTexture.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <memory>
#include <vector>

using PerspectiveCamera = raytracer::PerspectiveCamera;
using Ray = raytracer::Ray;
using Hittable = raytracer::Hittable;
using HitRecord = raytracer::HitRecord;
using BVH = raytracer::BVH;

//----------------------------------------------------------------------------------
void random_spheres()
{
    std::clog << "Rendering Scene 1: Random Spheres" << std::endl;
    BVH world;

    // Ground
    auto checkerTexture = std::make_shared<raytracer::CheckerTexture>(raytracer::Color3d(0.0, 0.0, 0.0), raytracer::Color3d(0.9, 0.9, 0.9), 2);
    auto materialGround = std::make_shared<raytracer::Lambertian>(checkerTexture);
    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0,-1000, 0), 1000, materialGround));

    // Random spheres
    for(int a=-11; a < 11; ++a)
    {
        for(int b=-11; b < 11; ++b)
        {
            auto chooseMat = raytracer::randomDouble();
            glm::dvec3 center(a + 0.9 * raytracer::randomDouble(), 0.2, b + 0.9 * raytracer::randomDouble());

            if(glm::length(center - glm::dvec3(4, 0.2, 0)) > 0.9)
            {
                std::shared_ptr<raytracer::Material> sphereMaterial;

                if(chooseMat < 0.8)
                {
                    // Diffuse
                    auto albedo = raytracer::randomVector() * raytracer::randomVector();
                    sphereMaterial = std::make_shared<raytracer::Lambertian>(albedo);
                    world.add(std::make_shared<raytracer::Sphere>(center, 0.2, sphereMaterial));
                }
                else if(chooseMat < 0.95)
                {
                    // Metal
                    auto albedo = raytracer::randomVector(0.5, 1);
                    auto fuzz = raytracer::randomDouble(0, 0.5);
                    sphereMaterial = std::make_shared<raytracer::Metal>(albedo, fuzz);
                    world.add(std::make_shared<raytracer::Sphere>(center, 0.2, sphereMaterial));
                }
                else
                {
                    // Glass
                    sphereMaterial = std::make_shared<raytracer::Dielectric>(1.5);
                    world.add(std::make_shared<raytracer::Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    // Three big spheres
    auto material1 = std::make_shared<raytracer::Dielectric>(1.5);
    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0, 1, -2), 1.0, material1));

    auto material2 = std::make_shared<raytracer::Lambertian>(raytracer::Color3d(0.4, 0.2, 0.1));
    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(-4, 1, -2), 1.0, material2));

    auto material3 = std::make_shared<raytracer::Metal>(raytracer::Color3d(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(4, 1, -2), 1.0, material3));

    // Build BVH
    world.build();

    PerspectiveCamera camera(1200, 675, 50, 20.0);
    // PerspectiveCamera camera(400, 200, 20, 20.0);
    // camera.setPosition(glm::dvec3(0, 0.3, 6));
    camera.setPosition(glm::dvec3(13,2,3));
    camera.setFocalPoint(glm::dvec3(0.0, 0.0, 0.0));
    camera.setAperatureRadius(0);
    // camera.tilt(-2);

    camera.render(world, 100);
}

//----------------------------------------------------------------------------------
void two_spheres()
{
    std::clog << "Rendering Scene 2: Two Spheres" << std::endl;
    BVH world;

    // Ground
    auto checkerTexture = std::make_shared<raytracer::CheckerTexture>(raytracer::Color3d(1.0, 0, 0), raytracer::Color3d(0.9, 0.9, 0.9), 0.8);

    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0, -10, 0), 10, std::make_shared<raytracer::Lambertian>(checkerTexture)));
    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0, 10, 0), 10, std::make_shared<raytracer::Lambertian>(checkerTexture)));

    // Build BVH
    world.build();

    // width, height, maxDepth, fovy
    PerspectiveCamera camera(400, 225, 50, 45);
    // camera.setPosition(glm::dvec3(0, 0, 20));
    camera.setPosition(glm::dvec3(13, 2, 3));
    camera.setFocalPoint(glm::dvec3(0, 0, 0));
    camera.setAperatureRadius(0);

    camera.render(world, 10);
}

//----------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // check if user provided -h or --help
    if(argc == 2)
    {
        if(std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")
        {
            std::clog << "Usage: raytracer <-h || -s scene_number>" << std::endl;
            std::clog << "-h --help: show help" << std::endl;
            std::clog << "-s 1: random_spheres" << std::endl;
            std::clog << "-s 2: two_spheres" << std::endl;
            return 0;
        }
    }

    if((argc == 3) && std::string(argv[1]) == "-s")
    {
        const int scene = std::stoi(argv[2]);

        switch (scene)
        {
        case 1:
            random_spheres();
            break;
        case 2:
            two_spheres();
            break;
        }
    }
    else
    {
        std::clog << "Usage: raytracer <-h || -s scene_number>" << std::endl;
        std::clog << "-h --help: show help" << std::endl;
        std::clog << "-s 1: random_spheres" << std::endl;
        std::clog << "-s 2: two_spheres" << std::endl;
    }

    return 0;
}

// int main()
// {
//     const int IMAGE_WIDTH = 400;
//     const int IMAGE_HEIGHT = 200;

//     // Materials
//     auto materialGround = std::make_shared<raytracer::Lambertian>(glm::dvec3(0.0, 0.8, 0.0));
//     auto materialCenter = std::make_shared<raytracer::Lambertian>(glm::dvec3(0.7, 0.3, 0.3));
//     auto materialLeft = std::make_shared<raytracer::Dielectric>(1.5);
//     auto materialRight = std::make_shared<raytracer::Metal>(glm::dvec3(0.8, 0.6, 0.2));

//     // World
//     raytracer::HittableList world;
//     world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0,0,-50), 9, materialCenter));
//     world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(20,0,-50), 9, materialLeft));
//     // world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(20,0,-50), -8, materialLeft));
//     world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(-20,0,-50), 9, materialRight));
//     world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0,-550.5,-200), 550, materialGround));

//     // // Camera
//     PerspectiveCamera camera(400, 200);
//     // // camera.dolly(30);
//     // // camera.boom(-50);
//     // // camera.tilt(-70);

//     camera.setFocalPoint(glm::dvec3(0,0,-50));
//     camera.setAperatureRadius(4);

//     camera.render(world, 10);

//     // std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

//     // for(int j=0; j < IMAGE_HEIGHT; ++j)
//     // {
//     //     std::clog << "\rScanlines remaining: " << IMAGE_HEIGHT - j << ' ' << std::flush;

//     //     for(int i=0; i < IMAGE_WIDTH; ++i)
//     //     {
//     //         std::unique_ptr<Ray> ray(camera.generateRay(glm::dvec2(i, j)));
//     //         auto pixelColor = rayColor(ray.get(), 10, world);
//     //         camera.writeColor3(std::cout, pixelColor, 1);
//     //     }
//     // }

//     // std::clog << "\nDone.\n";
//     return 0;
// }