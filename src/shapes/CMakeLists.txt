set (SHAPES_SRCS
        Sphere.cpp
        Quad.cpp
        Box.cpp)

add_library(shapes OBJECT ${SHAPES_SRCS})

target_include_directories(shapes
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../core
        ${CMAKE_CURRENT_SOURCE_DIR}/../materials)
