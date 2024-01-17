set (SRCS main.cpp)

# add_subdirectory(<sourcedir> [<binarydir>])
# Utility
add_subdirectory(utility)

# Core
add_subdirectory(core)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    stb_image glm::glm utility)