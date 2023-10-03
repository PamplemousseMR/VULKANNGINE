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
        VulkanNgine/Image.cpp
        VulkanNgine/Instance.cpp
        VulkanNgine/LogicalDevice.cpp
        VulkanNgine/PhysicalDevice.cpp
        VulkanNgine/RenderPass.cpp
        VulkanNgine/Semaphore.cpp
        VulkanNgine/ShaderModule.cpp
        VulkanNgine/Surface.cpp
        VulkanNgine/SwapChain.cpp
        VulkanNgine/SwapChainImageViews.cpp
        VulkanNgine/Texture.cpp
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
        VulkanNgine/Image.hpp
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
        VulkanNgine/Texture.hpp
        VulkanNgine/Window.hpp)

set(VKNGINE_PUBLIC_LINKS
        glfw
        glm::glm
        Vulkan::Vulkan)

set(VKNGINE_PRIVATE_LINKS
        stb)

vkngine_add_library(TARGET ${VKNGINE_LIB_NAME}  
                    WARNING_AS_ERROR
                    SOURCES ${VKNGINE_SRC} 
                    HEADERS ${VKNGINE_HEADER} 
                    PUBLIC_LINKS ${VKNGINE_PUBLIC_LINKS} 
                    PRIVATE_LINKS ${VKNGINE_PRIVATE_LINKS})