set (LIGHT_SRCS
    QuadLight.cpp
    SphereLight.cpp)

add_library(lights OBJECT ${LIGHT_SRCS})

target_include_directories(lights
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../core
        ${CMAKE_CURRENT_SOURCE_DIR}/../textures
        ${CMAKE_CURRENT_SOURCE_DIR}/../materials
        ${CMAKE_CURRENT_SOURCE_DIR}/../shapes)
