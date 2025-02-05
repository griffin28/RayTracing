set (SRCS main.cpp)

# add_subdirectory(<sourcedir> [<binarydir>])
# Utility
add_subdirectory(utility)

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

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
        Threads::Threads
    PRIVATE
        utility
        core
        cameras
        shapes
        materials
        textures
        stb_image glm::glm utility)