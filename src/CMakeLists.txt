set (SRCS main.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRCS})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE stb_image glm::glm)