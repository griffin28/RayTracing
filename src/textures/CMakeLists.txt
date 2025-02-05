set (TEXTURE_SRCS
    Texture.h
    SolidColorTexture.h
    CheckerTexture.h)

add_library(textures OBJECT ${TEXTURE_SRCS})

target_include_directories(textures
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../core)
