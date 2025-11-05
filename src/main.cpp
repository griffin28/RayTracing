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
using RaytracingUtility = raytracer::RaytracingUtility;

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
            auto chooseMat = static_cast<float>(RaytracingUtility::randomDouble());
            glm::vec3 center(a + 0.9f * static_cast<float>(RaytracingUtility::randomDouble()), 0.2f, b + 0.9f * static_cast<float>(RaytracingUtility::randomDouble()));

            if(glm::length(center - glm::vec3(4.f, 0.2f, 0.f)) > 0.9f)
            {
                std::shared_ptr<raytracer::Material> sphereMaterial;

                if(chooseMat < 0.8f)
                {
                    // Diffuse
                    auto albedo = RaytracingUtility::randomVector() * RaytracingUtility::randomVector();
                    sphereMaterial = std::make_shared<raytracer::Lambertian>(albedo);
                    world.add(std::make_shared<raytracer::Sphere>(center, 0.2f, sphereMaterial));
                }
                else if(chooseMat < 0.95f)
                {
                    // Metal
                    auto albedo = RaytracingUtility::randomVector(0.5f, 1.f);
                    auto fuzz = static_cast<float>(RaytracingUtility::randomDouble(0, 0.5));
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
    auto red = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(1.f, 0.f, 0.f));
    auto white = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.73f, 0.73f, 0.73f));
    auto green = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.f, 1.f, 0.f));
    auto blue = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.f, 0.f, 1.f));

    // Light
    auto quad = std::make_shared<raytracer::Quad>(glm::vec3(113,554,127), glm::vec3(330,0,0), glm::vec3(0,0,305));
    // auto quad = std::make_shared<raytracer::Quad>(glm::vec3(213, 554, 227), glm::vec3(130,0,0), glm::vec3(0,0,105));
    auto quadLight = std::make_shared<raytracer::QuadLight>(quad, raytracer::Color3f(1.f), 1.f);
    world.add(quadLight);

    // Walls
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(555,0,0), glm::vec3(0,555,0), glm::vec3(0,0,555), green));
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(0,0,0), glm::vec3(0,555,0), glm::vec3(0,0,555), red));
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(0,0,0), glm::vec3(555,0,0), glm::vec3(0,0,555), white)); // bottom 
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(555,555,555), glm::vec3(-555,0,0), glm::vec3(0,0,-555), white)); // top wall
    world.add(std::make_shared<raytracer::Quad>(glm::vec3(0,0,0), glm::vec3(555,0,0), glm::vec3(0,555,0), white)); // back wall

    // auto box2 = std::make_shared<raytracer::Box>(glm::vec3(265,0,295), glm::vec3(430,165,460), white);
    auto box2 = std::make_shared<raytracer::Box>(glm::vec3(0,0,0), glm::vec3(165,330,165), white);
    // box2->rotate(15, glm::vec3(0,1,0));
    box2->translate(glm::vec3(80,0,165));
    world.add(box2);

    // Boxes
    auto box1 = std::make_shared<raytracer::Box>(glm::vec3(0,0,0), glm::vec3(165,165,165), white);
    // box1->rotate(18, glm::vec3(0,1,0));
    box1->translate(glm::vec3(265,0,295));
    world.add(box1);
    // auto box1Sides = box1->getSides();
    // for(const auto &side : box1Sides)
    //     world.add(std::make_shared<raytracer::Quad>(side));

    // Build BVH
    world.build();

    // width, height, maxDepth, fovy
    PerspectiveCamera camera(600, 600, 40, 40);
    camera.setPosition(glm::vec3(278, 278, 1200));
    camera.setFocalPoint(glm::vec3(278, 278, 0));
    camera.setAperatureRadius(0);

    camera.render(world, 10);
}

//----------------------------------------------------------------------------------
void final_scene(const std::string &filename)
{
    std::clog << "Rendering Scene 7: Final Scene" << std::endl;
    BVH world;
    
    // Ground
    auto groundMaterial = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.48f, 0.83f, 0.53f));
    for(int i=0; i<20; ++i)
    {
        for(int j=0; j<20; ++j)
        {
            float w = 100.f;
            float x0 = -1000.f + i * w;
            float z0 = -1000.f + j * w;
            float y0 = 0.f;
            float x1 = x0 + w;
            float z1 = z0 + w;
            float y1 = static_cast<float>(RaytracingUtility::randomDouble(1, 101));
            
            auto box = std::make_shared<raytracer::Box>(glm::vec3(x0, y0, z0), glm::vec3(x1, y1, z1), groundMaterial);
            world.add(box);
        }
    }
    
    // Light
    auto quad = std::make_shared<raytracer::Quad>(glm::vec3(123, 554, 100), glm::vec3(330,0,0), glm::vec3(0,0,265));
    auto quadLight = std::make_shared<raytracer::QuadLight>(quad, raytracer::Color3f(1.0f), 1.0f);
    world.add(quadLight);
    
    // Spheres
    auto sphere1 = std::make_shared<raytracer::Sphere>(glm::vec3(400, 400, 200), 50, std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.7f, 0.3f, 0.1f)));
    world.add(sphere1);
    
    auto sphere2 = std::make_shared<raytracer::Sphere>(glm::vec3(240, 150, 355), 50, std::make_shared<raytracer::Dielectric>(1.5f));
    world.add(sphere2);
    
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0, 150, 255), 50, std::make_shared<raytracer::Metal>(raytracer::Color3f(0.8f, 0.8f, 0.9f), 1.0f)));
    
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(380,150,255), 70, std::make_shared<raytracer::Dielectric>(1.5f)));
    
    auto earthTexture = std::make_shared<raytracer::ImageTexture>(filename.c_str());
    auto earthMaterial = std::make_shared<raytracer::Lambertian>(earthTexture);
    auto sphere3 = std::make_shared<raytracer::Sphere>(glm::vec3(500,200, 0), 100, earthMaterial);
    world.add(sphere3);
    
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(220,280,100), 80, std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.8f, 0.5f, 0.2f))));
    
    // Sphere Box
    auto whiteMaterial = std::make_shared<raytracer::Lambertian>(raytracer::Color3f(0.73f, 0.73f, 0.73f));
    for(int i=0;i<1000; i++)
    {
        auto sphere = std::make_shared<raytracer::Sphere>(RaytracingUtility::randomVector(0,165), 10, whiteMaterial);
        sphere->translate(glm::vec3(50.f, 270.f, -150.0f));
        world.add(sphere);
    }
    
    // Build BVH
    world.build();

    // width, height, maxDepth, fovy
    PerspectiveCamera camera(800, 800, 40, 40);
    camera.setPosition(glm::vec3(78, 278, 1200));
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
    std::clog << "-s 7 -f filename: final scene" << std::endl;
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
        case 7:
            if((argc == 5) && std::string(argv[3]) == "-f")
            {
                final_scene(std::string(argv[4]));  
            }
            else
            {
                std::clog << "Usage: raytracer -s 7 -f <filename>" << std::endl;
            }
            break;
        default:
            std::clog << "Invalid scene number. Please use -h or --help for usage." << std::endl;
        }
    }
    else
    {
        print_usage();
    }

    return 0;
}