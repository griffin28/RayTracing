# RayTracing

![Screenshot from 2025-02-10 00-58-02](https://github.com/user-attachments/assets/2793adda-372e-4728-a77f-ebeba90460e4)

Code enhancements for Ray Tracing Series including Camera, BVH and other classes.

# Build
```bash
mkdir build && cd build
cmake [-DCMAKE_BUILD_TYPE=Debug] </path/to/raytracer>
cmake --build . [-j <threads>]
```

# Run
```bash
cd build
bin/raytracing <-h | -s scene_number>
```
* `-h --help`: show help
* `-s 1`: random spheres
* `-s 2`: two spheres
