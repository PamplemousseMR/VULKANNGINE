#=======================================
#                EXECUTABLE
#=======================================

set(VKNGINE_APP_NAME VKViewer)

set(VKNGINE_SRC
        CommandPool.cpp
        FrameBuffer.cpp
        Instance.cpp
        LogicalDevice.cpp
        main.cpp
        PhysicalDevice.cpp
        Pipeline.cpp
        RenderPass.cpp
        ShaderModule.cpp
        Surface.cpp
        SwapChain.cpp
        SwapChainImageViews.cpp
        Window.cpp)

set(VKNGINE_HEADER
        CommandPool.hpp
        FrameBuffer.hpp
        Instance.hpp
        logger.hpp
        LogicalDevice.hpp
        Pipeline.hpp
        RenderPass.hpp
        PhysicalDevice.hpp
        ShaderModule.hpp
        Surface.hpp
        SwapChain.hpp
        SwapChainImageViews.hpp
        Window.hpp)

set(VKNGINE_SHADER
        Shaders/default.vert
        Shaders/default.frag)

add_executable(${VKNGINE_APP_NAME} ${VKNGINE_SRC} ${VKNGINE_HEADER} ${VKNGINE_SHADER})

target_link_libraries(${VKNGINE_APP_NAME} PRIVATE glfw glm::glm Vulkan::Vulkan stdc++fs)

target_compile_options(${VKNGINE_APP_NAME} PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
    -Wall
    -Wextra
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wmisleading-indentation
    -Wdouble-promotion
    -Wformat=2
    -Wno-format-y2k
    -Werror
    -Wno-unused-parameter
    $<$<NOT:$<BOOL:${APPLE}>>:
    -Wduplicated-cond
    -Wduplicated-branches
    -Wuseless-cast
    -Wlogical-op>>)

add_shader(${VKNGINE_APP_NAME} SOURCES ${VKNGINE_SHADER})
