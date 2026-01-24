#include "PerspectiveCamera.h"
#include "HittablePdf.h"
#include "CosinePdf.h"
#include "MixturePdf.h"
#include "Sphere.h"
#include "Box.h"
#include "Quad.h"
#include "QuadLight.h"

#include <glm/ext/matrix_clip_space.hpp> // glm::perspective

#include <thread>
#include <functional> // std::bind
#include <fstream>
#include <iomanip>

namespace raytracer
{
//----------------------------------------------------------------------------------
PerspectiveCamera::PerspectiveCamera()
    : PerspectiveCamera(800, 600, 10)
{
}

//----------------------------------------------------------------------------------
PerspectiveCamera::PerspectiveCamera(int width,
                                     int height,
                                     int maxDepth,
                                     float fovy,
                                     float near,
                                     float far) :
    m_width(width),
    m_height(height),
    m_maxDepth(maxDepth),
    m_zoomFactor(1.0),
    m_fovy(fovy),
    m_near(near),
    m_far(far)
{
    if(m_width <= 0 || m_height <= 0)
    {
        throw std::invalid_argument("Invalid screen size");
    }

    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::reset()
{
    // TODO: save original values to reset to previous values
    this->Camera::reset();

    m_fovy = 45.0;
    m_near = 0.1;
    m_far = 1000.0;
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::render(const BVH &world, const int samplesPerPixel, std::ostream &out)
{
    std::unique_ptr<uint8_t[]> image(new uint8_t[m_width * m_height * 3]);

    const int sqrtspp = static_cast<int>(std::sqrt(samplesPerPixel));
    const float pixelSamplesScale = 1.0f / (sqrtspp * sqrtspp);

    auto numThreads = std::thread::hardware_concurrency() * 4;
    numThreads = numThreads > m_height ? m_height : numThreads;
    std::clog << "Using " << numThreads << " threads\n";
    std::vector<std::thread> threads(numThreads);

    for(int t=0; t<numThreads; t++)
    {
        // std::bind is used to pass the parameters to the lambda function
        threads[t] = std::thread(std::bind([&](int start, int end, int t)
        {
            for(int j=start; j < end; ++j)
            {
                if(t == static_cast<int>((numThreads / 2)))
                {
                    std::clog << "\rScanlines remaining: " << end - j << ' ' << std::flush;
                }

                for(int i=0; i < m_width; ++i)
                {
                    Color3f pixelColor(0.0f);

                    for(int sj = 0; sj < sqrtspp; ++sj)
                    {
                        for(int si = 0; si < sqrtspp; ++si)
                        {
                            // auto pixel = glm::vec2(i + RaytracingUtility::randomDouble() - 0.5f, j + RaytracingUtility::randomDouble() - 0.5f);
                            auto offset = this->sampleSquareStratified(si, sj, samplesPerPixel);
                            auto pixel = glm::vec2(i + offset.x, j + offset.y);
                            pixel += glm::vec2(0.5f, 0.5f); // Center of the pixel
                            std::unique_ptr<Ray> ray(this->generateThinLensRay(pixel));
                            // std::clog << "Generated Ray: " << *ray << "\n";
                            pixelColor += this->rayColor(ray.get(), m_maxDepth, world);
                        }
                    }

                    pixelColor *= pixelSamplesScale;
                    // Replace nan components with zero
                    if(std::isnan(pixelColor.r)) pixelColor.r = 0.0f;
                    if(std::isnan(pixelColor.g)) pixelColor.g = 0.0f;
                    if(std::isnan(pixelColor.b)) pixelColor.b = 0.0f;

                    pixelColor = glm::clamp(RaytracingUtility::gammaCorrect(pixelColor), 0.0f, 1.0f);

                    image[(j * m_width + i) * 3 + 0] = static_cast<uint8_t>(255.0f * pixelColor.r);
                    image[(j * m_width + i) * 3 + 1] = static_cast<uint8_t>(255.0f * pixelColor.g);
                    image[(j * m_width + i) * 3 + 2] = static_cast<uint8_t>(255.0f * pixelColor.b);
                }
            }
        }, t * m_height / numThreads, (t+1) == numThreads ? m_height : (t+1) * m_height / numThreads, t));
    }

    for(auto &thread : threads)
    {
        thread.join();
    }

    this->writePPMImage(image.get(), m_width, m_height, out);
    std::clog << "\nDone.\n";
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::zoom(const float factor)
{
    m_zoomFactor = factor;
    this->setViewAngle(m_fovy / factor);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setViewAngle(const float angle)
{
    if(m_fovy != angle)
    {
        float min = 0.00000001f;
        float max = 179.0f;

        m_fovy = (angle < min ? min : (angle > max ? max : angle));
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setClippingRange(const float near, const float far)
{
    if(m_near != near || m_far != far)
    {
        m_near = near;
        m_far = far;
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::setScreenSize(const int width, const int height)
{
    if(m_width != width || m_height != height)
    {
        m_width = width;
        m_height = height;
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

        m_perspectiveMatrix = glm::perspective(glm::radians(m_fovy), aspect, m_near, m_far);
    }

}

//----------------------------------------------------------------------------------
// Ray *
// PerspectiveCamera::generateThinLensRay(const glm::vec2 &pixel)
// {
//     Ray *pinholeRay = this->generateRay(pixel);
//     const float apertureRadius = this->getApertureRadius();

//     if(apertureRadius <= 0.0f)
//     {
//         return pinholeRay;
//     }

//     auto u = this->getHorizontalAxis();
//     auto v = this->getVerticalAxis();

//     auto defocusDiskU = u * apertureRadius;
//     auto defocusDiskV = v * apertureRadius;

//     glm::vec2 lensOffset = glm::vec2(randomInUnitDisk());

//     auto origin = this->getPosition() + (lensOffset[0] * defocusDiskU) + (lensOffset[1] * defocusDiskV);
//     glm::vec3 direction = glm::normalize(pinholeRay->direction() - origin);

//     // Clean-up
//     delete pinholeRay;

//     Ray *lensRay = new Ray(origin, direction);
//     return lensRay;
// }

//----------------------------------------------------------------------------------
Ray *
PerspectiveCamera::generateThinLensRay(const glm::vec2 &pixel)
{
    Ray *pinholeRay = this->generateRay(pixel);
    const float apertureRadius = this->getApertureRadius();

    if(apertureRadius <= 0.0f)
    {
        return pinholeRay;
    }

    glm::vec2 lensOffset = glm::vec2(RaytracingUtility::randomInUnitDisk());

    const float focalDistance = glm::distance(this->getWorldPosition(), this->getFocalPoint());
    const float fstop = focalDistance / (apertureRadius * 2.0f);

    float theta = lensOffset.x * apertureRadius * 2.0f * glm::pi<float>();
    float radius = lensOffset.y;

    float u = glm::cos(theta) * glm::sqrt(radius);
    float v = glm::sin(theta) * glm::sqrt(radius);

    glm::vec3 focusPoint = pinholeRay->direction() * (focalDistance / glm::dot(pinholeRay->direction(), this->getForwardAxis()));
    const float circleOfConfusionRadius = focalDistance / (2.0f * fstop);

    // glm::vec3 origin = this->getPosition() + (lensOffsetWorld * circleOfConfusion);
    glm::vec3 origin = this->getPosition() + (u * circleOfConfusionRadius * this->getHorizontalAxis()) + (v * circleOfConfusionRadius * this->getVerticalAxis());
    glm::vec3 direction = glm::normalize(focusPoint - origin);

    // Clean-up
    delete pinholeRay;

    Ray *lensRay = new Ray(origin, direction);
    return lensRay;
}

//----------------------------------------------------------------------------------
Ray *
PerspectiveCamera::generateRay(const glm::vec2 &pixel)
{
    // Raster Space -> Normalized Device Coordinate Space [-1,1]
    float pxN = pixel.x / static_cast<float>(m_width-1);
    float pyN = pixel.y / static_cast<float>(m_height-1);

    float pxNDC = 2 * pxN - 1;
    float pyNDC = 1 - 2 * pyN;

    // NDC Space -> Viewport Plane Coordinates in Camera Space   
    const float verticalHalfSize = tan(glm::radians(m_fovy * 0.5f));
    float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
    const float horizontalHalfSize = aspect * verticalHalfSize;

    float pxView = pxNDC * horizontalHalfSize * m_near;
    float pyView = pyNDC * verticalHalfSize * m_near;
    float pzView = m_near;

    // Camera coordinate frame
    auto u = this->getHorizontalAxis();
    auto v = this->getVerticalAxis();
    auto w = this->getForwardAxis();

    // Camera Space -> World Space    
    glm::vec3 cameraOriginWorld = this->getWorldPosition();
    glm::vec3 cameraDirectionWorld = glm::normalize((pxView * u) + (pyView * v) + (pzView * w));

    Ray *ray = new Ray(cameraOriginWorld, cameraDirectionWorld);
    return ray;
}

//----------------------------------------------------------------------------------
void
PerspectiveCamera::copy(const ProjectionCamera * const camera)
{
    if(camera != nullptr)
    {
        // Projection Camera
        this->zoom(camera->getZoomFactor());

        glm::vec2 clippingRange = camera->getClippingRange();
        this->setClippingRange(clippingRange[0], clippingRange[1]);

        glm::vec2 screenSize = camera->getScreenSize();
        this->setScreenSize(screenSize.x, screenSize.y);

        // Camera
        this->setPosition(camera->getPosition());
        this->setFocalPoint(camera->getFocalPoint());
        this->setViewUp(camera->getViewUp());

        this->setCameraToWorldMatrix(camera->getCameraToWorldMatrix());
    }
    else
    {
        this->reset();
    }
}

//----------------------------------------------------------------------------------
Color3f PerspectiveCamera::rayColor(Ray * const ray, int depth, const BVH &world)
{
    if(depth <= 0)
    {
        // std::clog << "Max ray depth reached\n";
        return Color3f(0.0f);
    }

    HitRecord record;

    if(world.hit(*ray, record))
    {        
        Color3f emitted = record.material->emitted(record);
        ScatterRecord scatterRecord;
        Ray scattered;
        float pdfValue = 1.0f;
        float scatteringPDF = 1.0f;

        if(!record.material->scatter(*ray, record, scatterRecord))
        {
            return emitted;
        }

        if(scatterRecord.skipPdf)
        {
            return scatterRecord.attenuation * rayColor(&scatterRecord.skipPdfRay, depth-1, world);
        }

        this->scatterRay(ray, world, record, scatterRecord, scattered, pdfValue, scatteringPDF);
        
        Color3f colorFromScatter = (scatterRecord.attenuation * scatteringPDF * rayColor(&scattered, depth-1, world)) / pdfValue;        
        return emitted + colorFromScatter;
    }

    // std::clog << "Ray miss - returning background color\n";
    return this->getBackgroundColor();
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::scatterRay(Ray * const ray, 
                                   const BVH &world, 
                                   const HitRecord &record,
                                   ScatterRecord &scatterRecord,
                                   Ray &scattered,
                                   float &pdf,
                                   float &scatteringPDF)
{
    

    
    std::vector<std::shared_ptr<Pdf>> pdfs;
    pdfs.push_back(scatterRecord.pdfPtr);

    auto lightSources = world.getLightSources();
    for(const auto &light : lightSources)
    {
        pdfs.push_back(std::make_shared<HittablePdf>(light, record.point));
    }

    MixturePdf mixturePdf(pdfs);
    scattered = Ray(record.point, glm::normalize(mixturePdf.generate()));
    pdf = mixturePdf.value(scattered.direction());
    scatteringPDF = record.material->scatteringPDF(*ray, record, scattered);
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::writePPMImage(uint8_t *image, const int width, const int height, std::ostream &out)
{
    out << "P3\n" << width << ' ' << height << "\n255\n";

    for(int j=0; j < height; j++)
    {
        for(int i=0; i < width; ++i)
        {
            out << static_cast<int>(image[(j * width + i) * 3 + 0]) << ' '
                << static_cast<int>(image[(j * width + i) * 3 + 1]) << ' '
                << static_cast<int>(image[(j * width + i) * 3 + 2]) << '\n';
        }
    }
}

//----------------------------------------------------------------------------------
void PerspectiveCamera::visualizeRayPaths(const std::string &filename,
                                          const BVH &world,
                                          int gridResolution,
                                          bool useThinLens,
                                          float missRayLength)
{
    std::ofstream outFile(filename);
    if(!outFile.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // Create MTL file for materials
    std::string mtlFilename = filename.substr(0, filename.find_last_of('.')) + ".mtl";
    std::ofstream mtlFile(mtlFilename);
    if(mtlFile.is_open())
    {
        mtlFile << "# Material file for ray visualization\n\n";
        mtlFile << "newmtl red_rays\n";
        mtlFile << "Ka 1.0 0.0 0.0\n";
        mtlFile << "Kd 1.0 0.0 0.0\n";
        mtlFile << "Ks 0.0 0.0 0.0\n";
        mtlFile << "illum 1\n\n";
        mtlFile << "newmtl white_objects\n";
        mtlFile << "Ka 1.0 1.0 1.0\n";
        mtlFile << "Kd 1.0 1.0 1.0\n";
        mtlFile << "Ks 0.2 0.2 0.2\n";
        mtlFile << "illum 2\n\n";
        mtlFile << "newmtl yellow_emissive\n";
        mtlFile << "Ka 1.0 1.0 0.0\n";
        mtlFile << "Kd 1.0 1.0 0.0\n";
        mtlFile << "Ke 1.0 1.0 0.0\n";
        mtlFile << "Ks 0.5 0.5 0.0\n";
        mtlFile << "illum 2\n";
        mtlFile.close();
    }

    std::clog << "Tracing and saving ray paths (" << gridResolution << "x" << gridResolution << ")...\n";

    outFile << "# Ray Path and Scene Visualization\n";
    outFile << "# Each line represents a ray segment between interactions\n";
    outFile << "# Camera Position: " << this->getWorldPosition().x << " "
            << this->getWorldPosition().y << " "
            << this->getWorldPosition().z << "\n";
    outFile << "mtllib " << mtlFilename.substr(mtlFilename.find_last_of('/') + 1) << "\n\n";

    outFile << std::fixed << std::setprecision(6);

    int vertexIndex = 0;
    const float cylinderRadius = 0.3f; // Radius of ray cylinders
    const int cylinderSegments = 8;     // Number of segments around cylinder

    // Helper lambda to create a cylinder between two points
    auto createCylinder = [&](glm::vec3 start, glm::vec3 end, int &vIndex) {
        glm::vec3 direction = glm::normalize(end - start);
        float length = glm::distance(start, end);
        
        // Create perpendicular vectors for cylinder base
        glm::vec3 up = glm::abs(glm::dot(direction, glm::vec3(0, 1, 0))) < 0.9f 
                        ? glm::vec3(0, 1, 0) 
                        : glm::vec3(1, 0, 0);
        glm::vec3 right = glm::normalize(glm::cross(direction, up));
        up = glm::normalize(glm::cross(right, direction));
        
        // Write start ring vertices
        int startRingIndex = vIndex + 1;
        for(int i = 0; i < cylinderSegments; ++i)
        {
            float angle = 2.0f * glm::pi<float>() * i / cylinderSegments;
            glm::vec3 offset = cylinderRadius * (glm::cos(angle) * right + glm::sin(angle) * up);
            glm::vec3 v = start + offset;
            outFile << "v " << v.x << " " << v.y << " " << v.z << "\n";
            vIndex++;
        }
        
        // Write end ring vertices
        int endRingIndex = vIndex + 1;
        for(int i = 0; i < cylinderSegments; ++i)
        {
            float angle = 2.0f * glm::pi<float>() * i / cylinderSegments;
            glm::vec3 offset = cylinderRadius * (glm::cos(angle) * right + glm::sin(angle) * up);
            glm::vec3 v = end + offset;
            outFile << "v " << v.x << " " << v.y << " " << v.z << "\n";
            vIndex++;
        }
        
        // Write side faces connecting the two rings
        for(int i = 0; i < cylinderSegments; ++i)
        {
            int next = (i + 1) % cylinderSegments;
            int v1 = startRingIndex + i;
            int v2 = startRingIndex + next;
            int v3 = endRingIndex + next;
            int v4 = endRingIndex + i;
            
            outFile << "f " << v1 << " " << v2 << " " << v3 << "\n";
            outFile << "f " << v1 << " " << v3 << " " << v4 << "\n";
        }
    };

    // Use red material for rays
    outFile << "\n# Ray segments as cylinders (red)\n";
    outFile << "usemtl red_rays\n";

    for(int row = 0; row < gridResolution; ++row)
    {
        for(int col = 0; col < gridResolution; ++col)
        {
            float pixelX = (col / static_cast<float>(gridResolution - 1)) * (m_width - 1);
            float pixelY = (row / static_cast<float>(gridResolution - 1)) * (m_height - 1);
            glm::vec2 pixel(pixelX+0.5f, pixelY+0.5f); // Center of pixel

            std::unique_ptr<Ray> ray(useThinLens ? this->generateThinLensRay(pixel)
                                                 : this->generateRay(pixel));

            int depth = m_maxDepth;
            glm::vec3 currentOrigin = ray->origin();

            while(depth-- > 0)
            {
                HitRecord record;
                if(!world.hit(*ray, record))
                {
                    // Miss: draw cylinder segment out to a fixed length for context
                    glm::vec3 missEnd = currentOrigin + ray->direction() * missRayLength;
                    createCylinder(currentOrigin, missEnd, vertexIndex);
                    break;
                }

                // Hit: draw cylinder segment to the intersection point
                createCylinder(currentOrigin, record.point, vertexIndex);

                // Generate scattered ray to continue path
                ScatterRecord scatterRecord;
                Ray scattered;
                float pdfValue = 1.0f;
                float scatteringPDF = 1.0f;

                if(!record.material->scatter(*ray, record, scatterRecord))
                {
                    break; // absorption or emission-only material
                }

                if(scatterRecord.skipPdf)
                {
                    scattered = scatterRecord.skipPdfRay;
                }
                else
                {   
                    this->scatterRay(ray.get(), world, record, scatterRecord, scattered, pdfValue, scatteringPDF);
                }

                currentOrigin = record.point;
                ray.reset(new Ray(scattered));
            }
        }
    }

    // Optional: add camera and focal point markers
    glm::vec3 camPos = this->getWorldPosition();
    outFile << "\n# Camera position\n";
    outFile << "v " << camPos.x << " " << camPos.y << " " << camPos.z << "\n";
    vertexIndex++;

    glm::vec3 focalPt = this->getFocalPoint();
    outFile << "\n# Focal point\n";
    outFile << "v " << focalPt.x << " " << focalPt.y << " " << focalPt.z << "\n";
    vertexIndex++;

    // Add scene objects with appropriate materials
    outFile << "\n# Scene Objects (white and emissive)\n";
    
    const auto& sceneObjects = world.getSceneObjects();
    int objCount = 0;
    
    for(const auto& obj : sceneObjects)
    {
        outFile << "\n# Object " << (++objCount) << "\n";
        
        // Check if object is a QuadLight first (emissive)
        if(auto quadLight = std::dynamic_pointer_cast<QuadLight>(obj))
        {
            outFile << "# QuadLight (yellow emissive)\n";
            outFile << "usemtl yellow_emissive\n";
            for(int corner=0; corner < 4; ++corner)
            {
                glm::vec3 p = quadLight->getCorner(corner);
                outFile << "v " << p.x << " " << p.y << " " << p.z << "\n";
                vertexIndex++;
            }

            int quadVertexStart = vertexIndex - 3; // Last 4 vertices added
            
            // 2 triangles
            outFile << "f " << quadVertexStart << " " << (quadVertexStart+1) << " " << (quadVertexStart+2) << "\n";
            outFile << "f " << quadVertexStart << " " << (quadVertexStart+2) << " " << (quadVertexStart+3) << "\n";
        }
        // Check if object is a sphere
        else if(auto sphere = std::dynamic_pointer_cast<Sphere>(obj))
        {
            outFile << "# Sphere (white)\n";
            outFile << "usemtl white_objects\n";
            glm::vec3 c = sphere->center();
            AxisAlignedBoundingBox bounds = sphere->getBounds();
            glm::vec3 size = bounds.pMax() - bounds.pMin();
            float radius = size.x * 0.5f;
            
            // Generate sphere mesh (UV sphere)
            int segments = 16;
            int rings = 12;
            int sphereVertexStart = vertexIndex + 1; // +1 for 1-based indexing
            
            for(int ring = 0; ring <= rings; ++ring)
            {
                float phi = glm::pi<float>() * static_cast<float>(ring) / static_cast<float>(rings);
                for(int seg = 0; seg <= segments; ++seg)
                {
                    float theta = 2.0f * glm::pi<float>() * static_cast<float>(seg) / static_cast<float>(segments);
                    
                    float x = radius * sin(phi) * cos(theta);
                    float y = radius * cos(phi);
                    float z = radius * sin(phi) * sin(theta);
                    
                    outFile << "v " << (c.x + x) << " " << (c.y + y) << " " << (c.z + z) << "\n";
                    vertexIndex++;
                }
            }
            
            // Generate faces
            for(int ring = 0; ring < rings; ++ring)
            {
                for(int seg = 0; seg < segments; ++seg)
                {
                    int current = sphereVertexStart + ring * (segments + 1) + seg;
                    int next = current + segments + 1;
                    
                    outFile << "f " << current << " " << (current + 1) << " " << (next + 1) << "\n";
                    outFile << "f " << current << " " << (next + 1) << " " << next << "\n";
                }
            }
        }
        // Check if object is a box
        else if(auto box = std::dynamic_pointer_cast<Box>(obj))
        {
            outFile << "# Box (white)\n";
            outFile << "usemtl white_objects\n";
            auto points = box->getWorldPoints();
            int boxVertexStart = vertexIndex + 1;
            for(const auto& p : points)
            {
                outFile << "v " << p.x << " " << p.y << " " << p.z << "\n";
                vertexIndex++;
            }

            // 12 faces (2 triangles per side)
            int v = boxVertexStart;
            outFile << "f " << v << " " << (v+1) << " " << (v+2) << "\n";
            outFile << "f " << v << " " << (v+2) << " " << (v+3) << "\n";
            // Back face
            outFile << "f " << (v+4) << " " << (v+6) << " " << (v+5) << "\n";
            outFile << "f " << (v+4) << " " << (v+7) << " " << (v+6) << "\n";
            // Left face
            outFile << "f " << (v+4) << " " << v << " " << (v+3) << "\n";
            outFile << "f " << (v+4) << " " << (v+3) << " " << (v+7) << "\n";
            // Right face
            outFile << "f " << (v+1) << " " << (v+5) << " " << (v+6) << "\n";
            outFile << "f " << (v+1) << " " << (v+6) << " " << (v+2) << "\n";
            // Top face
            outFile << "f " << (v+3) << " " << (v+2) << " " << (v+6) << "\n";
            outFile << "f " << (v+3) << " " << (v+6) << " " << (v+7) << "\n";
            // Bottom face
            outFile << "f " << (v+4) << " " << (v+5) << " " << (v+1) << "\n";
            outFile << "f " << (v+4) << " " << (v+1) << " " << v << "\n";
        }
        // Check if object is a quad
        else if(auto quad = std::dynamic_pointer_cast<Quad>(obj))
        {
            outFile << "# Quad (white)\n";
            outFile << "usemtl white_objects\n";
            for(int corner=0; corner < 4; ++corner)
            {
                glm::vec3 p = quad->getCorner(corner);
                outFile << "v " << p.x << " " << p.y << " " << p.z << "\n";
                vertexIndex++;
            }

            int quadVertexStart = vertexIndex - 3; // Last 4 vertices added
            
            // 2 triangles
            outFile << "f " << quadVertexStart << " " << (quadVertexStart+1) << " " << (quadVertexStart+2) << "\n";
            outFile << "f " << quadVertexStart << " " << (quadVertexStart+2) << " " << (quadVertexStart+3) << "\n";
        }
    }
    
    std::clog << "Included " << objCount << " scene objects in ray path visualization\n";
    outFile.close();
    std::clog << "Ray paths saved to " << filename << "\n";
}

} // namespace raytracer