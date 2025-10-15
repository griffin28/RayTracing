set (SRCS main.cpp)

# add_subdirectory(<sourcedir> [<binarydir>])
# Core
add_subdirectory(core)

# Cameras (perspective, orthographic)
add_subdirectory(cameras)

# Shapes (spheres)
add_subdirectory(shapes)

# Materials
add_subdirectory(materials)

# Textures
add_subdirectory(textures)

# Lights
add_subdirectory(lights)

# PDFs
add_subdirectory(pdfs)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
        Threads::Threads
    PRIVATE
        core
        cameras
        shapes
        materials
        textures
        lights
        pdfs
        stb_image
        glm::glm)