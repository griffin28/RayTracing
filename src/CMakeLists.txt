set (SRCS main.cpp)

# add_subdirectory(<sourcedir> [<binarydir>])
# Utility
add_subdirectory(utility)

# Core
add_subdirectory(core)

# Cameras
add_subdirectory(cameras)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        utility
        core
        cameras
        stb_image glm::glm utility)