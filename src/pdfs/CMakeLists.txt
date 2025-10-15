set (PDF_SRCS
     SpherePdf.h
     CosinePdf.h)

add_library(pdfs OBJECT ${PDF_SRCS})

target_include_directories(pdfs
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${GLM_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/../core)
