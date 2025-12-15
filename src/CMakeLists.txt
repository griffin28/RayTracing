set (SRCS main.cpp)

# add_subdirectory(<sourcedir> [<binarydir>])
# Core
add_subdirectory(core)

# Cameras (perspective, orthographic)
add_subdirectory(cameras)

# Shapes (spheres)
add_subdirectory(shapes)

# PDFs
add_subdirectory(pdfs)

# Materials
add_subdirectory(materials)

# Textures
add_subdirectory(textures)

# Lights
add_subdirectory(lights)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
        Threads::Threads
    PRIVATE
        core
        cameras
        shapes
        pdfs
        materials
        textures
        lights
        stb_image
        glm::glm)