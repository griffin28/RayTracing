set (MATERIAL_SRCS
    Lambertian.cpp
    Metal.cpp
    Dielectric.cpp
    EmissiveMaterial.cpp)

add_library(materials OBJECT ${MATERIAL_SRCS})

target_include_directories(materials
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../core
        ${CMAKE_CURRENT_SOURCE_DIR}/../textures
        ${CMAKE_CURRENT_SOURCE_DIR}/../pdfs)
