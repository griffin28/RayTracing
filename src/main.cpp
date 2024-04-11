#include "PerspectiveCamera.h"
#include "Sphere.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

#include <iostream>
#include <memory>

using PerspectiveCamera = raytracer::PerspectiveCamera;

int main()
{
    // Materials
    // auto materialGround = std::make_shared<raytracer::Lambertian>(glm::dvec3(0.8, 0.8, 0.0));
    // auto materialCenter = std::make_shared<raytracer::Lambertian>(glm::dvec3(0.7, 0.3, 0.3));
    // auto materialLeft = std::make_shared<raytracer::Dielectric>(glm::dvec3(0.8, 0.8, 0.8), 1.5);
    // auto materialRight = std::make_shared<raytracer::Metal>(glm::dvec3(0.8, 0.6, 0.2));

    // // World
    // raytracer::HittableList world;
    // world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0,0,-50), 10, materialCenter));
    // world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(20,0,-50), 10, materialLeft));
    // world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(-20,0,-50), 10, materialRight));
    // world.add(std::make_shared<raytracer::Sphere>(glm::dvec3(0,-150.5,-1), 5000, materialGround));

    // // Camera
    PerspectiveCamera camera(256, 256);
    // // camera.dolly(30);
    // // camera.boom(-50);
    // // camera.tilt(-70);

    // camera.setAperatureRadius(0.0);

    // camera.render(world, 5);

    std::cout << "P3\n" << 256 << ' ' << 256 << "\n255\n";

    for(int j=0; j < 256; ++j)
    {
        std::clog << "\rScanlines remaining: " << 256 - j << ' ' << std::flush;
        for(int i=0; i < 256; ++i)
        {
            glm::dvec3 pixelColor = glm::dvec3(static_cast<double>(i) / 255.0, static_cast<double>(j) / 255.0, 0.25);

            camera.writeColor3(std::cout, pixelColor, 1);
        }
    }

    std::clog << "\nDone.\n";
    return 0;
}