#=======================================
#                EXECUTABLE
#=======================================

set(VKNGINE_APP_NAME Basic)

set(VKNGINGE_SOURCES 
    main.cpp
    Pipeline.hpp 
    Pipeline.cpp)

 set(VKNGINGE_SHADERS 
    basic.vert 
    basic.frag)

 set(VKNGINGE_LINKS 
    VKNgine)

vkngine_add_executable(TARGET ${VKNGINE_APP_NAME} SOURCES ${VKNGINGE_SOURCES} SHADERS ${VKNGINGE_SHADERS} LINKS ${VKNGINGE_LINKS})