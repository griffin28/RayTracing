set (CORE_SRCS
        Ray.h
        Hittable.h
        Utility.h
        BVH.cpp
        AABB.cpp
        ImageLoader.cpp)

add_library(core OBJECT ${CORE_SRCS})

target_compile_options(core
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)

target_include_directories(core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../materials
        ${CMAKE_CURRENT_LIST_DIR}/../../external/stb_image)