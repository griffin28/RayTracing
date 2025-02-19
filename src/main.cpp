#include "PerspectiveCamera.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
#include "Utility.h"
#include "Ray.h"
#include "BVH.h"
#include "CheckerTexture.h"
#include "ImageTexture.h"

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

    camera.render(world, 3);
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

    camera.render(world, 5);
}

//----------------------------------------------------------------------------------
void earth()
{
    std::clog << "Rendering Scene 3: Earth" << std::endl;
    BVH world;

    // Ground
    auto earthTexture = std::make_shared<raytracer::ImageTexture>("earth_8k.jpg");
    auto earthMaterial = std::make_shared<raytracer::Lambertian>(earthTexture);
    world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0, 0, 0), 2, earthMaterial));

    // Build BVH
    world.build();

    // width, height, maxDepth, fovy
    PerspectiveCamera camera(400, 225, 50, 20);
    camera.setPosition(glm::dvec3(0, 0, 12));
    camera.setFocalPoint(glm::dvec3(0, 0, 0));
    camera.setAperatureRadius(0);

    camera.render(world, 50);
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
            std::clog << "-s 3: earth" << std::endl;
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
        case 3:
            earth();
            break;
        }
    }
    else
    {
        std::clog << "Usage: raytracer <-h || -s scene_number>" << std::endl;
        std::clog << "-h --help: show help" << std::endl;
        std::clog << "-s 1: random_spheres" << std::endl;
        std::clog << "-s 2: two_spheres" << std::endl;
        std::clog << "-s 3: earth" << std::endl;
    }

    return 0;
}