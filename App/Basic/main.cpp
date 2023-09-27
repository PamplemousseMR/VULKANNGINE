#include "VulkanNgine/Buffer.hpp"
#include "VulkanNgine/CommandBuffers.hpp"
#include "VulkanNgine/CommandPool.hpp"
#include "VulkanNgine/DeviceMemory.hpp"
#include "VulkanNgine/Fence.hpp"
#include "VulkanNgine/FrameBuffer.hpp"
#include "VulkanNgine/Instance.hpp"
#include "VulkanNgine/LogicalDevice.hpp"
#include "VulkanNgine/PhysicalDevice.hpp"
#include "VulkanNgine/Pipeline.hpp"
#include "VulkanNgine/RenderPass.hpp"
#include "VulkanNgine/Semaphore.hpp"
#include "VulkanNgine/ShaderModule.hpp"
#include "VulkanNgine/Surface.hpp"
#include "VulkanNgine/SwapChain.hpp"
#include "VulkanNgine/SwapChainImageViews.hpp"
#include "VulkanNgine/Window.hpp"
#include "VulkanNgine/logger.hpp"

#include <cstdlib>
#include <iostream>

constexpr size_t MAX_FRAMES_IN_FLIGHT = 3;

int main()
{
    Window window("Basic");

    Instance instance(window);

    Surface surface(window, instance);

    std::vector<PhysicalDevice> physicalDevices = PhysicalDevice::getDevices(instance, surface);

    std::vector<PhysicalDevice>::const_iterator selectedDevice =
      std::find_if(physicalDevices.begin(), physicalDevices.end(), [&](const PhysicalDevice& _device) {
          // Check the  graphic queue
          const std::vector<PhysicalDevice::QueueFamily>::const_iterator graphicQueueFamily =
            std::find_if(_device.getQueueFamilies().begin(),
                         _device.getQueueFamilies().end(),
                         [&](const PhysicalDevice::QueueFamily& queueFamily) {
                             if(queueFamily.m_graphics)
                             {
                                 return true;
                             }
                             return false;
                         });

          // Check the swap chain
          bool swapChainAdequate = false;
          if(_device.hasSwapChainSupport())
          {
              PhysicalDevice::SwapChainSupportDetails details = _device.getSwapChainSupportDetails();
              swapChainAdequate = !details.m_formats.empty() && !details.m_presentModes.empty();
          }

          return graphicQueueFamily != _device.getQueueFamilies().end() && swapChainAdequate;
      });

    if(selectedDevice == physicalDevices.end())
    {
        VKNGINE_LOG_ERROR("No physical device found");
        return -1;
    }

    VKNGINE_LOG_VERBOSE("Selected physical device " << selectedDevice->getName());
    for(PhysicalDevice::QueueFamily queueFamily : selectedDevice->getQueueFamilies())
    {
        VKNGINE_LOG_VERBOSE("   Queue family " << queueFamily.m_index);
        VKNGINE_LOG_VERBOSE("       Graphics " << queueFamily.m_graphics);
        VKNGINE_LOG_VERBOSE("       Compute " << queueFamily.m_compute);
        VKNGINE_LOG_VERBOSE("       Transfer " << queueFamily.m_transfer);
        VKNGINE_LOG_VERBOSE("       Sparse " << queueFamily.m_sparseBinding);
        VKNGINE_LOG_VERBOSE("       Protected " << queueFamily.m_protected);
        VKNGINE_LOG_VERBOSE("       Present " << queueFamily.m_present);
    }

    LogicalDevice logicalDevice(*selectedDevice);

    const std::vector<Buffer::Vertex> vertices = {
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

    Buffer vertexBuffer(logicalDevice, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertices);

    DeviceMemory vertexMemory(logicalDevice,
                              *selectedDevice,
                              vertexBuffer,
                              VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vertexMemory.mapMemory();

    SwapChain swapChain(*selectedDevice, surface, logicalDevice);

    SwapChainImageViews swapChainImageViews(logicalDevice, swapChain);

    ShaderModule defaultVertShaderModule(logicalDevice, "basic.vert.bin");
    ShaderModule defaultFragShaderModule(logicalDevice, "basic.frag.bin");

    RenderPass renderPass(logicalDevice, swapChain.getFormat().format);

    Pipeline pipeline(
      logicalDevice, defaultVertShaderModule, defaultFragShaderModule, renderPass, swapChain.getExtent());

    std::vector<FrameBuffer> framebuffers;
    framebuffers.reserve(swapChainImageViews.get().size());

    for(size_t i = 0; i < swapChainImageViews.get().size(); ++i)
    {
        framebuffers.emplace_back(logicalDevice, renderPass, swapChainImageViews.get()[i], swapChain.getExtent());
    }

    CommandPool commandPool(logicalDevice, VK_QUEUE_GRAPHICS_BIT);

    CommandBuffers commandBuffers(logicalDevice, commandPool, swapChainImageViews.get().size());

    for(size_t i = 0; i < swapChainImageViews.get().size(); ++i)
    {
        VkCommandBufferBeginInfo commandBufferBeginInfo{};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags = 0;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        if(vkBeginCommandBuffer(commandBuffers.get()[i], &commandBufferBeginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin command buffer");
        }

        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = renderPass.get();
        renderPassBeginInfo.framebuffer = framebuffers[i].get();
        renderPassBeginInfo.renderArea.offset = {0, 0};
        renderPassBeginInfo.renderArea.extent = swapChain.getExtent();

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers.get()[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers.get()[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());

        VkBuffer buffers[] = {vertexBuffer.get()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers.get()[i], 0, 1, buffers, offsets);

        vkCmdDraw(commandBuffers.get()[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers.get()[i]);

        if(vkEndCommandBuffer(commandBuffers.get()[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to end command buffer");
        }
    }

    std::vector<Semaphore> imageAvailableSemaphores;
    std::vector<Semaphore> renderFinishedSemaphores;
    std::vector<Fence> inFlightFences;
    std::vector<Fence*> imagesInFlight;

    imageAvailableSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.reserve(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.reserve(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChain.getImages().size(), VK_NULL_HANDLE);

    for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        imageAvailableSemaphores.emplace_back(logicalDevice);
        renderFinishedSemaphores.emplace_back(logicalDevice);
        inFlightFences.emplace_back(logicalDevice);
    }

    size_t currentFrame = 0;

    while(!window.shouldClose())
    {
        window.poolEvent();

        uint32_t imageIndex;
        vkAcquireNextImageKHR(logicalDevice.get(),
                              swapChain.get(),
                              UINT64_MAX,
                              imageAvailableSemaphores.at(currentFrame).get(),
                              VK_NULL_HANDLE,
                              &imageIndex);

        if(imagesInFlight.at(imageIndex) != VK_NULL_HANDLE)
        {
            imagesInFlight.at(imageIndex)->wait();
        }

        imagesInFlight[imageIndex] = &(inFlightFences.at(currentFrame));

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores.at(currentFrame).get()};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &(commandBuffers.get()[imageIndex]);

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores.at(currentFrame).get()};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        inFlightFences.at(currentFrame).reset();

        if(vkQueueSubmit(logicalDevice.getGraphicQueue(), 1, &submitInfo, inFlightFences.at(currentFrame).get()) !=
           VK_SUCCESS)
        {
            throw std::runtime_error("Failed to submit queue");
        }

        VkPresentInfoKHR presentInfoKHR{};
        presentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfoKHR.waitSemaphoreCount = 1;
        presentInfoKHR.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChainsKHR[] = {swapChain.get()};
        presentInfoKHR.swapchainCount = 1;
        presentInfoKHR.pSwapchains = swapChainsKHR;
        presentInfoKHR.pImageIndices = &imageIndex;
        presentInfoKHR.pResults = nullptr;

        vkQueuePresentKHR(logicalDevice.getPresentQueue(), &presentInfoKHR);
        // vkQueueWaitIdle(logicalDevice.getPresentQueue());

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    vkDeviceWaitIdle(logicalDevice.get());

    // Why VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT in VkSubmitInfo and VkSubpassDependency ? same information
    // Why CommandBuffer from CommandPool from Queue, to use the same queue in vkQueueSubmit who use the CommandBuffer ?

    // IV-D-3-h. Rendu en cours

    return EXIT_SUCCESS;
}
