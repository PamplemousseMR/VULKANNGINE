#=======================================
#                EXECUTABLE
#=======================================

set(VKNGINE_APP_NAME Texture)

set(VKNGINGE_SOURCES 
    main.cpp
    Pipeline.hpp 
    Pipeline.cpp)

 set(VKNGINGE_SHADERS 
    texture.vert 
    texture.frag)

 set(VKNGINGE_LINKS 
    VKNgine)

vkngine_add_executable(TARGET ${VKNGINE_APP_NAME} SOURCES ${VKNGINGE_SOURCES} SHADERS ${VKNGINGE_SHADERS} LINKS ${VKNGINGE_LINKS})