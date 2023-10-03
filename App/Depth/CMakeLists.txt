#=======================================
#                EXECUTABLE
#=======================================

set(VKNGINE_APP_NAME Depth)

set(VKNGINGE_SOURCES 
    main.cpp
    Pipeline.hpp 
    Pipeline.cpp)

 set(VKNGINGE_SHADERS 
    depth.vert 
    depth.frag)

 set(VKNGINGE_LINKS 
    VKNgine
    glm::glm)
    
 set(VKNGINGE_RESOURCES
    texture.jpeg)

vkngine_add_executable(TARGET ${VKNGINE_APP_NAME} SOURCES ${VKNGINGE_SOURCES} SHADERS ${VKNGINGE_SHADERS} LINKS ${VKNGINGE_LINKS} RESOURCES ${VKNGINGE_RESOURCES})