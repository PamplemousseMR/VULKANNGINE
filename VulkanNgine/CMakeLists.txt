#=======================================
#                LIBRARY
#=======================================

set(VKNGINE_LIB_NAME VKNgine)

set(VKNGINE_SRC
        VulkanNgine/Buffer.cpp
        VulkanNgine/CommandBuffers.cpp
        VulkanNgine/CommandPool.cpp
        VulkanNgine/DescriptorPool.cpp
        VulkanNgine/DescriptorSets.cpp
        VulkanNgine/DeviceMemory.cpp
        VulkanNgine/Fence.cpp
        VulkanNgine/FrameBuffer.cpp
        VulkanNgine/Instance.cpp
        VulkanNgine/LogicalDevice.cpp
        VulkanNgine/PhysicalDevice.cpp
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
        VulkanNgine/DescriptorPool.hpp
        VulkanNgine/DescriptorSets.hpp
        VulkanNgine/DeviceMemory.hpp
        VulkanNgine/Fence.hpp
        VulkanNgine/FrameBuffer.hpp
        VulkanNgine/Instance.hpp
        VulkanNgine/logger.hpp
        VulkanNgine/LogicalDevice.hpp
        VulkanNgine/RenderPass.hpp
        VulkanNgine/Semaphore.hpp
        VulkanNgine/PhysicalDevice.hpp
        VulkanNgine/ShaderModule.hpp
        VulkanNgine/Surface.hpp
        VulkanNgine/SwapChain.hpp
        VulkanNgine/SwapChainImageViews.hpp
        VulkanNgine/Window.hpp)

vkngine_add_library(TARGET ${VKNGINE_LIB_NAME} SOURCES ${VKNGINE_SRC} HEADERS ${VKNGINE_HEADER} PUBLIC_LINKS glfw glm::glm Vulkan::Vulkan)