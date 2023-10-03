#=======================================
#                LIBRARY
#=======================================

set(VKNGINE_LIB_NAME stb)

set(VKNGINE_HEADERS
        stb/stb_image.h)

set(VKNGINE_SOURCES
        stb/stb_image.c)

vkngine_add_library(TARGET ${VKNGINE_LIB_NAME} HEADERS ${VKNGINE_HEADERS} SOURCES ${VKNGINE_SOURCES})

set_target_properties(${VKNGINE_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
