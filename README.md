![Screenshot from 2025-02-19 00-54-01](https://github.com/user-attachments/assets/f8da45ba-845e-4e40-8c8d-9e34fab608ec)

# RayTracing
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
bin/raytracing <-s scene_number> [-h] [-f filename]
```
* `-h --help`: show help
* `-s 1`: random spheres
* `-s 2`: two spheres
* `-s 3 -f filename`: earth
* `-s 4`: quads
* `-s 5 -f filename`: quad and sphere lights
* `-s 6`: cornell box
