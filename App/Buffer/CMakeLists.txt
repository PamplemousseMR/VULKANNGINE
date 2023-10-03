#=======================================
#                EXECUTABLE
#=======================================

set(VKNGINE_APP_NAME Buffer)

set(VKNGINGE_SOURCES 
    main.cpp
    Pipeline.hpp 
    Pipeline.cpp)

 set(VKNGINGE_SHADERS 
    buffer.vert 
    buffer.frag)

 set(VKNGINGE_LINKS 
    VKNgine
    glm::glm)

vkngine_add_executable(TARGET ${VKNGINE_APP_NAME} SOURCES ${VKNGINGE_SOURCES} SHADERS ${VKNGINGE_SHADERS} LINKS ${VKNGINGE_LINKS})