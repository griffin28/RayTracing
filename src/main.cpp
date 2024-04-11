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
    auto materialGround = std::make_shared<raytracer::Lambertian>(glm::vec3(0.8, 0.8, 0.0));
    auto materialCenter = std::make_shared<raytracer::Lambertian>(glm::vec3(0.7, 0.3, 0.3));
    auto materialLeft = std::make_shared<raytracer::Dielectric>(glm::vec3(0.8, 0.8, 0.8), 1.5f);
    auto materialRight = std::make_shared<raytracer::Metal>(glm::vec3(0.8, 0.6, 0.2));

    // World
    raytracer::HittableList world;
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0,0,-50), 10, materialCenter));
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(20,0,-50), 10, materialLeft));
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(-20,0,-50), 10, materialRight));
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0,-150.5,-1), 5000, materialGround));

    // Camera
    PerspectiveCamera camera(400, 300, 50, 45.f);
    // camera.dolly(30);
    // camera.boom(-50);
    // camera.tilt(-70);

    camera.setAperatureRadius(1.f);

    camera.render(world, 5);

    return 0;
}