#=======================================
#                LIBRARY
#=======================================

set(VKNGINE_LIB_NAME VKNgine)

set(VKNGINE_SRC
        VulkanNgine/Buffer.cpp
        VulkanNgine/CommandBuffers.cpp
        VulkanNgine/CommandPool.cpp
        VulkanNgine/DeviceMemory.cpp
        VulkanNgine/Fence.cpp
        VulkanNgine/FrameBuffer.cpp
        VulkanNgine/Instance.cpp
        VulkanNgine/LogicalDevice.cpp
        VulkanNgine/PhysicalDevice.cpp
        VulkanNgine/Pipeline.cpp
        VulkanNgine/RenderPass.cpp
        VulkanNgine/Semaphore.cpp
        VulkanNgine/ShaderModule.cpp
        VulkanNgine/Surface.cpp
        VulkanNgine/SwapChain.cpp
        VulkanNgine/SwapChainImageViews.cpp
        VulkanNgine/Window.cpp)

set(VKNGINE_HEADER
        VulkanNgine/Buffer.hpp
        VulkanNgine/CommandBuffers.hpp
        VulkanNgine/CommandPool.hpp
        VulkanNgine/DeviceMemory.hpp
        VulkanNgine/Fence.hpp
        VulkanNgine/FrameBuffer.hpp
        VulkanNgine/Instance.hpp
        VulkanNgine/logger.hpp
        VulkanNgine/LogicalDevice.hpp
        VulkanNgine/Pipeline.hpp
        VulkanNgine/RenderPass.hpp
        VulkanNgine/Semaphore.hpp
        VulkanNgine/PhysicalDevice.hpp
        VulkanNgine/ShaderModule.hpp
        VulkanNgine/Surface.hpp
        VulkanNgine/SwapChain.hpp
        VulkanNgine/SwapChainImageViews.hpp
        VulkanNgine/Window.hpp)

add_library(${VKNGINE_LIB_NAME} STATIC ${VKNGINE_SRC} ${VKNGINE_HEADER} ${VKNGINE_SHADER})

target_include_directories(${VKNGINE_LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${VKNGINE_LIB_NAME} PUBLIC glfw glm::glm Vulkan::Vulkan stdc++fs)

target_compile_options(${VKNGINE_LIB_NAME} PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
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
