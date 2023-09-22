#include "CommandBuffers.hpp"
#include "CommandPool.hpp"
#include "FrameBuffer.hpp"
#include "Instance.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
#include "Semaphore.hpp"
#include "ShaderModule.hpp"
#include "Surface.hpp"
#include "SwapChain.hpp"
#include "SwapChainImageViews.hpp"
#include "Window.hpp"
#include "logger.hpp"

#include <cstdlib>
#include <iostream>

int main()
{
    Window window("VulkanNgine");

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

    SwapChain swapChain(*selectedDevice, surface, logicalDevice);

    SwapChainImageViews swapChainImageViews(logicalDevice, swapChain);

    ShaderModule defaultVertShaderModule(logicalDevice, "Shaders/default.vert.bin");
    ShaderModule defaultFragShaderModule(logicalDevice, "Shaders/default.frag.bin");

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

        vkCmdDraw(commandBuffers.get()[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers.get()[i]);

        if(vkEndCommandBuffer(commandBuffers.get()[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to end command buffer");
        }
    }

    Semaphore imageAvailableSemaphore(logicalDevice);
    Semaphore renderFinishedSemaphore(logicalDevice);

    while(!window.shouldClose())
    {
        window.poolEvent();

        uint32_t imageIndex;
        vkAcquireNextImageKHR(
          logicalDevice.get(), swapChain.get(), UINT64_MAX, imageAvailableSemaphore.get(), VK_NULL_HANDLE, &imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore.get()};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &(commandBuffers.get()[imageIndex]);

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore.get()};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if(vkQueueSubmit(logicalDevice.getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
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
        vkQueueWaitIdle(logicalDevice.getPresentQueue());
    }

    vkDeviceWaitIdle(logicalDevice.get());

    // Why FrameBuffer and Pipeline needs the RenderPass since the VkRenderPassBeginInfo knows it, duplicated info:
    // https://www.reddit.com/r/vulkan/comments/kjzkqi/what_is_the_reason_we_reference_render_pass_in/
    // How to use pipeline in multiple RenderPass ?
    // Why VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT in VkSubmitInfo and VkSubpassDependency ? same information
    // Why CommandBuffer from CommandPool from Queue, to use the same queue in vkQueueSubmit who use the CommandBuffer ?

    // IV-D-3-h. Rendu en cours

    return EXIT_SUCCESS;
}
