#include "PerspectiveCamera.h"
#include "Sphere.h"
#include "Quad.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"
#include "Utility.h"
#include "Ray.h"
#include "BVH.h"
#include "CheckerTexture.h"
#include "ImageTexture.h"
#include "QuadLight.h"
#include "SphereLight.h"
#include "Box.h"

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
    auto checkerTexture = std::make_shared<raytracer::CheckerTexture>(raytracer::Color3f(0.0f, 0.0f, 0.0f), raytracer::Color3f(0.9f, 0.9f, 0.9f), 2);
    auto materialGround = std::make_shared<raytracer::Lambertian>(checkerTexture);
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0.f,-1000.f, 0.f), 1000, materialGround));

    // Random spheres
    for(int a=-11; a < 11; ++a)
    {
        for(int b=-11; b < 11; ++b)
        {
            auto chooseMat = static_cast<float>(raytracer::randomDouble());
            glm::vec3 center(a + 0.9f * static_cast<float>(raytracer::randomDouble()), 0.2f, b + 0.9f * static_cast<float>(raytracer::randomDouble()));

            if(glm::length(center - glm::vec3(4.f, 0.2f, 0.f)) > 0.9f)
            {
                std::shared_ptr<raytracer::Material> sphereMaterial;

                if(chooseMat < 0.8f)
                {
                    // Diffuse
                    auto albedo = raytracer::randomVector() * raytracer::randomVector();
                    sphereMaterial = std::make_shared<raytracer::Lambertian>(albedo);
                    world.add(std::make_shared<raytracer::Sphere>(center, 0.2f, sphereMaterial));
                }
                else if(chooseMat < 0.95f)
                {
                    // Metal
                    auto albedo = raytracer::randomVector(0.5f, 1.f);
                    auto fuzz = static_cast<float>(raytracer::randomDouble(0, 0.5));
                    sphereMaterial = std::make_shared<raytracer::Metal>(albedo, fuzz);
                    world.add(std::make_shared<raytracer::Sphere>(center, 0.2f, sphereMaterial));
                }
                else
                {
                    // Glass
                    sphereMaterial = std::make_shared<raytracer::Dielectric>(1.5f);
                    world.add(std::make_shared<raytracer::Sphere>(center, 0.2f, sphereMaterial));
                }
            }
        }
    }

    // Three big spheres
    auto material1 = std::make_shared<raytracer::Dielectric>(1.5f);
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0.f, 1.f, -2.f), 1.0f, material1));

    // auto material2 = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.4, 0.2, 0.1));
    auto earthTexture = std::make_shared<raytracer::ImageTexture>("earth_8k.jpg");
    auto earthMaterial = std::make_shared<raytracer::Lambertian>(earthTexture);
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(-4.f, 1.f, -2.f), 1.f, earthMaterial));

    auto material3 = std::make_shared<raytracer::Metal>(raytracer::Color3f(0.7f, 0.6f, 0.5f), 0.0f);
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(4.f, 1.f, -2.f), 1.f, material3));

    // Build BVH
    world.build();

    PerspectiveCamera camera(1200, 675, 5, 20.0f);
    camera.setBackgroundColor(raytracer::Color3f(0.7f, 0.8f, 1.f));
    // PerspectiveCamera camera(400, 200, 20, 20.0);
    // camera.setPosition(glm::vec3(0, 0.3, 6));
    camera.setPosition(glm::vec3(13.f, 2.f, 3.f));
    camera.setFocalPoint(glm::vec3(0.f, 0.f, 0.f));
    camera.setAperatureRadius(0.f);
    camera.setBackgroundColor(raytracer::Color3f(0.7f, 0.8f, 1.f));

    camera.render(world, 3);
}

//----------------------------------------------------------------------------------
void two_spheres()
{
    std::clog << "Rendering Scene 2: Two Spheres" << std::endl;
    BVH world;

    // Ground
    auto checkerTexture = std::make_shared<raytracer::CheckerTexture>(raytracer::Color3f(1.0f, 0.f, 0.f), raytracer::Color3f(0.9f, 0.9f, 0.9f), 0.8f);

    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0.f, -10.f, 0.f), 10.f, std::make_shared<raytracer::Lambertian>(checkerTexture)));
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0.f, 10.f, 0.f), 10.f, std::make_shared<raytracer::Lambertian>(checkerTexture)));

    // Build BVH
    world.build();

    // width, height, maxDepth, fovy
    PerspectiveCamera camera(400, 225, 50, 45.f);
    camera.setBackgroundColor(raytracer::Color3f(0.7f, 0.8f, 1.f)); // Light blue background
    camera.setPosition(glm::vec3(13.f, 2.f, 3.f));
    camera.setFocalPoint(glm::vec3(0.f, 0.f, 0.f));
    camera.setAperatureRadius(0.f);

    camera.render(world, 50);
}

//----------------------------------------------------------------------------------
void earth(const std::string &filename)
{
    std::clog << "Rendering Scene 3: Earth" << std::endl;
    BVH world;

    // Ground
    auto earthTexture = std::make_shared<raytracer::ImageTexture>(filename.c_str());
    auto earthMaterial = std::make_shared<raytracer::Lambertian>(earthTexture);
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0, 0, 0), 2, earthMaterial));

    // Build BVH
    world.build();

    // width, height, maxDepth, fovy
    PerspectiveCamera camera(400, 225, 50, 20);
    camera.setPosition(glm::vec3(0, 0, 12));
    camera.setFocalPoint(glm::vec3(0, 0, 0));
    camera.setAperatureRadius(0);

    camera.render(world, 5);
}

//----------------------------------------------------------------------------------
void quads()
{
    std::clog << "Rendering Scene 4: Quads" << std::endl;
    BVH world;

    // Materials
    auto leftRed = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(1.0f, 0.0f, 0.0f));
    auto backGreen = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.0f, 0.9f, 0.0f));
    auto rightBlue = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.1f, 0.0f, 1.0f));
    auto upperOrange = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(1.0f, 0.5f, 0.0f));
    auto lowerYellow = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.2f, 0.8f, 0.8f));

    // Quads
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(-3,-2,5), glm::vec3(0,0,-4), glm::vec3(0,4,0), leftRed));
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(-2,-2,0), glm::vec3(4,0,0), glm::vec3(0,4,0), backGreen));
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(3,-2,1), glm::vec3(0,0,4), glm::vec3(0,4,0), rightBlue));
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(-2,3,1), glm::vec3(4,0,0), glm::vec3(0,0,4), upperOrange));
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(-2,-3,5), glm::vec3(4,0,0), glm::vec3(0,0,-4), lowerYellow));

    // Build BVH
    world.build();

    PerspectiveCamera camera(400, 400, 50, 80);
    camera.setPosition(glm::vec3(0, 0, 9));
    camera.setFocalPoint(glm::vec3(0, 0, 0));
    camera.setAperatureRadius(0);

    camera.render(world, 25);
}

//----------------------------------------------------------------------------------
void simple_light(const std::string &filename)
{
    std::clog << "Rendering Scene 5: Quad and Sphere Lights" << std::endl;
    BVH world;

    // Earth
    auto earthTexture = std::make_shared<raytracer::ImageTexture>(filename.c_str());
    auto earthMaterial = std::make_shared<raytracer::Lambertian>(earthTexture);
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0, 2, 0), 2, earthMaterial));

    // Ground
    auto checkerTexture = std::make_shared<raytracer::CheckerTexture>(raytracer::Color3f(0.0f, 0.0f, 0.0f), raytracer::Color3f(0.9f, 0.9f, 0.9f), 2);
    auto materialGround = std::make_shared<raytracer::Lambertian>(checkerTexture);
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0,-1000, 0), 1000, materialGround));

    // Lights
    auto quad = std::make_shared<raytracer::Quad>(glm::vec3(3,1,-2), glm::vec3(2,0,0), glm::vec3(0,2,0));
    auto quadLight = std::make_shared<raytracer::QuadLight>(quad, raytracer::Color3f(1.0f), 1.0f);
    world.add(quadLight);

    auto sphere = std::make_shared<raytracer::Sphere>(glm::vec3(0,7,0), 2.0f);
    auto sphereLight = std::make_shared<raytracer::SphereLight>(sphere, raytracer::Color3f(1.0f, 0.4f, 0.6f), 1.0f);
    world.add(sphereLight);

    // Build BVH
    world.build();

    // width, height, maxDepth, fovy
    PerspectiveCamera camera(400, 225, 50, 20);
    camera.setPosition(glm::vec3(26,3,6));
    camera.setFocalPoint(glm::vec3(0, 2, 0));
    camera.setAperatureRadius(0);
    camera.setBackgroundColor(raytracer::Color3f(0.0f, 0.0f, 0.0f));

    camera.render(world, 50);
}

//----------------------------------------------------------------------------------
void cornell_box()
{
    std::clog << "Rendering Scene 6: Cornell Box" << std::endl;
    BVH world;

    // Materials
    auto red = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.65f, 0.05f, 0.05f));
    auto white = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.73f, 0.73f, 0.73f));
    auto green = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.12f, 0.45f, 0.0f));

    // Light
    auto quad = std::make_shared<raytracer::Quad>(glm::vec3(113,554,127), glm::vec3(330,0,0), glm::vec3(0,0,305));
    // auto quad = std::make_shared<raytracer::Quad>(glm::vec3(343, 554, 332), glm::vec3(-130,0,0), glm::vec3(0,0,-105));
    auto quadLight = std::make_shared<raytracer::QuadLight>(quad, raytracer::Color3f(1.0f), 1.0f);
    world.add(quadLight);

    // Walls
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(555,0,0), glm::vec3(0,555,0), glm::vec3(0,0,555), green));
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(0,0,0), glm::vec3(0,555,0), glm::vec3(0,0,555), red));
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(0,0,0), glm::vec3(555,0,0), glm::vec3(0,0,555), white));
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(555,555,555), glm::vec3(-555,0,0), glm::vec3(0,0,-555), white));
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(0,0,555), glm::vec3(555,0,0), glm::vec3(0,555,0), white));

    // Boxes
    auto box1 = std::make_shared<raytracer::Box>(glm::vec3(265,0,295), glm::vec3(430,330,460), white);
    // auto box1Sides = box1.getSides();
    // for(const auto &side : box1Sides)
    // {
    //     // Add the sides of the box to the world
    //     world.add(std::make_shared<raytracer::Quad>(side));
    // }
    // auto box1 = std::make_shared<raytracer::Box>(glm::vec3(130.f, 0.f, 65.f), glm::vec3(295.f, 165.f, 230.f), white);
    // box1->rotate(15.f, glm::vec3(0.f, 1.f, 0.f));
    // box1->translate(glm::vec3(265.f, 0.f, 295.f));
    world.add(box1);
    
    auto box2 = std::make_shared<raytracer::Box>(glm::vec3(130,0,65), glm::vec3(295,165,230), white);
    // auto box2 = raytracer::Box(glm::vec3(130,0,65), glm::vec3(295,165,230), white);
    //  auto box2Sides = box2.getSides();
    //  for(const auto &side : box2Sides)
    //  {
    //     // Add the sides of the box to the world
    //     world.add(std::make_shared<raytracer::Quad>(side));
    //  }
    // Uncomment the following lines to add a second box
    // auto box2 = std::make_shared<raytracer::Box>(glm::vec3(265,0,295), glm::vec3(430,330,460), white);
    // auto box2 = std::make_shared<raytracer::Box>(glm::vec3(0,0,0), glm::vec3(165,165,165), white);
    // box2->rotate(-18, glm::vec3(0,1,0));
    // box2->translate(glm::vec3(130,0,65));
    world.add(box2);

    // Build BVH
    world.build();

    // width, height, maxDepth, fovy
    PerspectiveCamera camera(600, 600, 40, 40);
    camera.setPosition(glm::vec3(278, 278, -800));
    camera.setFocalPoint(glm::vec3(278, 278, 0));
    camera.setAperatureRadius(0);

    camera.render(world, 10);
}

//----------------------------------------------------------------------------------
void print_usage()
{
    std::clog << "Usage: raytracing <-s scene_number> [-h] [-f filename]" << std::endl;
    std::clog << "-h --help: show help" << std::endl;
    std::clog << "-s 1: random_spheres" << std::endl;
    std::clog << "-s 2: two_spheres" << std::endl;
    std::clog << "-s 3 -f filename: earth" << std::endl;
    std::clog << "-s 4: quads" << std::endl;
    std::clog << "-s 5 -f filename: quad and sphere lights" << std::endl;
    std::clog << "-s 6: cornell box" << std::endl;
}

//----------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // check if user provided -h or --help
    if(argc == 2)
    {
        if(std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")
        {
            print_usage();
            return 0;
        }
    }

    if((argc >= 3) && std::string(argv[1]) == "-s")
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
            if((argc == 5) && std::string(argv[3]) == "-f")
            {
                earth(std::string(argv[4]));
            }
            else
            {
                std::clog << "Usage: raytracer -s 3 -f <filename>" << std::endl;
            }
            break;
        case 4:
            quads();
            break;
        case 5:
            if((argc == 5) && std::string(argv[3]) == "-f")
            {
                simple_light(std::string(argv[4]));
            }
            else
            {
                std::clog << "Usage: raytracer -s 5 -f <filename>" << std::endl;
            }
            break;
        case 6:
            cornell_box();
            break;
        }
    }
    else
    {
        print_usage();
    }

    return 0;
}