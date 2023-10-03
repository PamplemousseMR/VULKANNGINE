#=======================================
#                LIBRARY
#=======================================

set(VKNGINE_LIB_NAME stb)

set(VKNGINE_HEADER
        stb/stb_image.h)

vkngine_add_library(TARGET ${VKNGINE_LIB_NAME} TYPE INTERFACE HEADERS ${VKNGINE_HEADER})

set_target_properties(${VKNGINE_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
