set (UTILITY_SRCS Color.h)

add_library(utility OBJECT ${UTILITY_SRCS})

target_compile_options(utility
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-fPIC>)

target_include_directories(utility
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${GLM_INCLUDE_DIRS})