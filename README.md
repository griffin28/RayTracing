# 🌟 RayTracing

<p align="center">
  <img src="https://github.com/user-attachments/assets/f8da45ba-845e-4e40-8c8d-9e34fab608ec" alt="Ray Traced Scene" width="800"/>
</p>

A feature-rich, physically-based ray tracer written in modern C++14. This project implements advanced rendering techniques including BVH acceleration, importance sampling, and various material models for realistic image synthesis.

## ✨ Features
### Ray Path Visualization
<p align="center">
 <img width="800" alt="Screenshot from 2026-01-15 17-59-36" src="https://github.com/user-attachments/assets/a6c52290-768b-477d-8f06-154de483e104" />
</p>

Comprehensive ray path visualization system to export camera rays and scene geometry as 3D OBJ files for debugging and analysis.

### Rendering
- **Path Tracing** with configurable bounce depth and samples per pixel
- **BVH Acceleration** - Bounding Volume Hierarchy for efficient ray-scene intersection
- **Importance Sampling** with multiple PDF strategies (Cosine, Hittable, Mixture, Sphere)
- **Stratified & Halton Sequence Sampling** for reduced noise
- **Depth of Field** via thin lens camera model
- **Multi-threaded Rendering** for improved performance

### Materials
| Material | Description |
|----------|-------------|
| **Lambertian** | Diffuse surfaces with perfect scattering |
| **Metal** | Reflective surfaces with configurable fuzziness |
| **Dielectric** | Glass/transparent materials with Schlick's approximation |
| **Emissive** | Light-emitting materials for area lights |

### Geometry
- **Spheres** - With full UV mapping for textures
- **Quads** - Parallelogram primitives for walls, floors, and area lights
- **Boxes** - Constructed from quads with rotation and translation support

### Textures
- **Solid Color** - Constant color textures
- **Checker Pattern** - Procedural 3D checker texture
- **Image Textures** - Load external images (JPG, PNG, etc.)

### Lighting
- **Quad Lights** - Rectangular area lights
- **Sphere Lights** - Spherical area lights
- **Environment Lighting** - Configurable background color

### Camera System
- **Perspective Camera** with adjustable FOV and aspect ratio
- **Orthographic Camera** for parallel projection
- Camera controls: roll, tilt, pan, dolly, boom, zoom

## 🛠️ Dependencies

- **CMake** 3.20+
- **C++14** compatible compiler
- **GLM** (OpenGL Mathematics) - Included in `external/`
- **stb_image** - Included in `external/`

## 📦 Build

```bash
# Clone the repository
git clone https://github.com/griffin28/RayTracing.git
cd RayTracing

# Create build directory and configure
mkdir build && cd build
cmake [-DCMAKE_BUILD_TYPE=Release] ..

# Build the project
cmake --build . [-j $(nproc)]
```

## 🚀 Usage

```bash
cd build
bin/raytracing -s <scene_number> [-f <filename>] [-h]
```

### Options

| Flag | Description |
|------|-------------|
| `-h, --help` | Show help message |
| `-s <num>` | Select scene to render (1-7) |
| `-f <file>` | Specify texture image file (required for some scenes) |

### Available Scenes

| Scene | Command | Description |
|-------|---------|-------------|
| 1 | `-s 1` | Random spheres with various materials |
| 2 | `-s 2` | Two large spheres with checker texture |
| 3 | `-s 3 -f earth.jpg` | Earth sphere with image texture |
| 4 | `-s 4` | Colorful quads demonstration |
| 5 | `-s 5 -f earth.jpg` | Quad and sphere lights demo |
| 6 | `-s 6` | Classic Cornell Box |
| 7 | `-s 7 -f earth.jpg` | Final scene with 1000+ objects |

### Example

```bash
# Render the Cornell Box scene
bin/raytracing -s 6

# Render Earth with custom texture
bin/raytracing -s 3 -f /path/to/earth_8k.jpg
```

Output is written to `stdout` in PPM format. Redirect to save:

```bash
bin/raytracing -s 6 > cornell_box.ppm
```

## 🏗️ Project Structure

```
RayTracing/
├── cmake/                 # CMake modules
├── external/              # Third-party libraries (GLM, stb_image)
├── src/
│   ├── cameras/           # Camera implementations
│   │   ├── Camera.h/cpp
│   │   ├── PerspectiveCamera.h/cpp
│   │   └── OrthographicCamera.h/cpp
│   ├── core/              # Core ray tracing infrastructure
│   │   ├── AABB.h/cpp     # Axis-Aligned Bounding Box
│   │   ├── BVH.h/cpp      # Bounding Volume Hierarchy
│   │   ├── Ray.h          # Ray representation
│   │   └── Hittable.h     # Abstract hittable interface
│   ├── materials/         # Material models
│   │   ├── Lambertian.h/cpp
│   │   ├── Metal.h/cpp
│   │   └── Dielectric.h/cpp
│   ├── shapes/            # Geometric primitives
│   │   ├── Sphere.h/cpp
│   │   ├── Quad.h/cpp
│   │   └── Box.h/cpp
│   ├── textures/          # Texture implementations
│   │   ├── ImageTexture.h
│   │   └── CheckerTexture.h
│   ├── lights/            # Light sources
│   │   ├── QuadLight.h/cpp
│   │   └── SphereLight.h/cpp
│   ├── pdfs/              # Probability Density Functions
│   │   ├── CosinePdf.h
│   │   ├── HittablePdf.h
│   │   └── MixturePdf.h
│   └── main.cpp           # Entry point with scene definitions
└── CMakeLists.txt
```

## 📚 References

This implementation is inspired by and extends concepts from:

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [Ray Tracing: The Next Week](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
- [Ray Tracing: The Rest of Your Life](https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html)
- [Physically Based Rendering: From Theory to Implementation](https://www.pbrt.org/)

## 📄 License

This project is licensed under the [MIT License](LICENSE).

## 🤝🏿 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
