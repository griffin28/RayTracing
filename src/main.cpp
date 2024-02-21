#include "PerspectiveCamera.h"
#include "Sphere.h"

#include <iostream>
#include <memory>

using PerspectiveCamera = raytracer::PerspectiveCamera;

int main()
{
    // World
    raytracer::HittableList world;
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0,0,-50), 10));
    world.add(std::make_shared<raytracer::Sphere>(glm::vec3(0,-150.5,-1), 1000));

    // Camera
    PerspectiveCamera camera;
    camera.render(world, 6);

    return 0;
}