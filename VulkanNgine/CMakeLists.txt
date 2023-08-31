#=======================================
#                EXECUTABLE
#=======================================

set(VKNGINE_APP_NAME VKViewer)

set(VKNGINE_SRC
        App.cpp
        main.cpp)

set(VKNGINE_HEADER
        App.hpp)

add_executable(${VKNGINE_APP_NAME} ${VKNGINE_SRC} ${VKNGINE_HEADER})

target_link_libraries(${VKNGINE_APP_NAME} PRIVATE glfw glm::glm Vulkan::Vulkan)
