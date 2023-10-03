#include "Pipeline.hpp"
#include <VulkanNgine/Buffer.hpp>
#include <VulkanNgine/CommandBuffers.hpp>
#include <VulkanNgine/CommandPool.hpp>
#include <VulkanNgine/DescriptorPool.hpp>
#include <VulkanNgine/DescriptorSets.hpp>
#include <VulkanNgine/DeviceMemory.hpp>
#include <VulkanNgine/Fence.hpp>
#include <VulkanNgine/FrameBuffer.hpp>
#include <VulkanNgine/Instance.hpp>
#include <VulkanNgine/LogicalDevice.hpp>
#include <VulkanNgine/PhysicalDevice.hpp>
#include <VulkanNgine/RenderPass.hpp>
#include <VulkanNgine/Semaphore.hpp>
#include <VulkanNgine/ShaderModule.hpp>
#include <VulkanNgine/Surface.hpp>
#include <VulkanNgine/SwapChain.hpp>
#include <VulkanNgine/SwapChainImageViews.hpp>
#include <VulkanNgine/Window.hpp>
#include <VulkanNgine/logger.hpp>
#include <VulkanNgine/Texture.hpp>
#include <VulkanNgine/Image.hpp>
#include <VulkanNgine/ImageView.hpp>
#include <VulkanNgine/Sampler.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <cstdlib>
#include <iostream>

constexpr size_t MAX_FRAMES_IN_FLIGHT = 3;

struct UniformBufferObject
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

int main()
{
    Window window("Texture");

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

          return graphicQueueFamily != _device.getQueueFamilies().end() && swapChainAdequate && _device.hasSamplerAnisotropySupport();
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

    const std::vector<Pipeline::Vertex> vertices = {{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                                  {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                                  {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                                                  {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

    const VkDeviceSize vertexBufferSize = sizeof(vertices[0]) * vertices.size();

    Buffer vertexBuffer(
      logicalDevice, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBufferSize);

    DeviceMemory vertexMemory(logicalDevice, *selectedDevice, vertexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    CommandPool transferCommandPool(logicalDevice, VK_QUEUE_TRANSFER_BIT);

    {
        Buffer vertexStagingBuffer(logicalDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, vertexBufferSize);

        DeviceMemory vertexStagingMemory(logicalDevice,
                                         *selectedDevice,
                                         vertexStagingBuffer,
                                         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        vertexStagingMemory.mapMemory(vertices.data(), vertexStagingBuffer.getSize());

        CommandBuffers transferCommandBuffers(logicalDevice, transferCommandPool, 1);

        transferCommandBuffers.beginSingleTimeCommand(0);

        VkBufferCopy bufferCopy{};
        bufferCopy.srcOffset = 0;
        bufferCopy.dstOffset = 0;
        bufferCopy.size = vertexBufferSize;
        vkCmdCopyBuffer(transferCommandBuffers.get()[0], vertexStagingBuffer.get(), vertexBuffer.get(), 1, &bufferCopy);

        transferCommandBuffers.endSingleTimeCommand(0);
    }

    const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

    const VkDeviceSize indicesBufferSize = sizeof(vertices[0]) * vertices.size();

    Buffer indexBuffer(
      logicalDevice, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indicesBufferSize);

    DeviceMemory indexMemory(logicalDevice, *selectedDevice, indexBuffer, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    {
        Buffer indexStagingBuffer(logicalDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, indicesBufferSize);

        DeviceMemory indexStagingMemory(logicalDevice,
                                        *selectedDevice,
                                        indexStagingBuffer,
                                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        indexStagingMemory.mapMemory(indices.data(), indexStagingBuffer.getSize());

        CommandBuffers transferCommandBuffers(logicalDevice, transferCommandPool, 1);

        transferCommandBuffers.beginSingleTimeCommand(0);

        VkBufferCopy bufferCopy{};
        bufferCopy.srcOffset = 0;
        bufferCopy.dstOffset = 0;
        bufferCopy.size = indicesBufferSize;
        vkCmdCopyBuffer(transferCommandBuffers.get()[0], indexStagingBuffer.get(), indexBuffer.get(), 1, &bufferCopy);

        transferCommandBuffers.endSingleTimeCommand(0);
    }

    SwapChain swapChain(*selectedDevice, surface, logicalDevice);

    SwapChainImageViews swapChainImageViews(logicalDevice, swapChain);

    std::vector<Buffer> uniformBuffers;
    std::vector<DeviceMemory> uniformDeviceMemories;

    uniformBuffers.reserve(swapChainImageViews.get().size());
    uniformDeviceMemories.reserve(swapChainImageViews.get().size());

    for(size_t i = 0; i < swapChainImageViews.get().size(); ++i)
    {
        uniformBuffers.emplace_back(logicalDevice, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, sizeof(UniformBufferObject));
        uniformDeviceMemories.emplace_back(logicalDevice,
                                           *selectedDevice,
                                           uniformBuffers.at(i),
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    Texture texture("texture.jpeg");

    Image textureImage(
        logicalDevice, texture, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

    DeviceMemory textureMemory(logicalDevice, *selectedDevice, textureImage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    {
        Buffer textureStagingBuffer(logicalDevice, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, texture.getSize());

        DeviceMemory textureStagingMemory(logicalDevice,
            *selectedDevice,
            textureStagingBuffer,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        textureStagingMemory.mapMemory(texture.data(), textureStagingBuffer.getSize());

        {
            CommandBuffers transferCommandBuffers(logicalDevice, transferCommandPool, 1);

            transferCommandBuffers.beginSingleTimeCommand(0);

            VkImageMemoryBarrier imageMemoryBarrier{};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.image = textureImage.get();
            imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
            imageMemoryBarrier.subresourceRange.levelCount = 1;
            imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
            imageMemoryBarrier.subresourceRange.layerCount = 1;
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            vkCmdPipelineBarrier(transferCommandBuffers.get()[0], VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

            transferCommandBuffers.endSingleTimeCommand(0);
        }
        {
            CommandBuffers transferCommandBuffers(logicalDevice, transferCommandPool, 1);

            transferCommandBuffers.beginSingleTimeCommand(0);

            VkBufferImageCopy bufferImageCopy{};
            bufferImageCopy.bufferOffset = 0;
            bufferImageCopy.bufferRowLength = 0;
            bufferImageCopy.bufferImageHeight = 0;
            bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferImageCopy.imageSubresource.mipLevel = 0;
            bufferImageCopy.imageSubresource.baseArrayLayer = 0;
            bufferImageCopy.imageSubresource.layerCount = 1;
            bufferImageCopy.imageOffset = { 0, 0, 0 };
            bufferImageCopy.imageExtent = { static_cast<uint32_t>(texture.width()), static_cast<uint32_t>(texture.height()), 1 };

            vkCmdCopyBufferToImage(transferCommandBuffers.get()[0], textureStagingBuffer.get(), textureImage.get(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);

            transferCommandBuffers.endSingleTimeCommand(0);
        }
        {
            CommandBuffers transferCommandBuffers(logicalDevice, transferCommandPool, 1);

            transferCommandBuffers.beginSingleTimeCommand(0);

            VkImageMemoryBarrier imageMemoryBarrier{};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            imageMemoryBarrier.image = textureImage.get();
            imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
            imageMemoryBarrier.subresourceRange.levelCount = 1;
            imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
            imageMemoryBarrier.subresourceRange.layerCount = 1;
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(transferCommandBuffers.get()[0], VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

            transferCommandBuffers.endSingleTimeCommand(0);
        }
    }

    ImageView textureImageView(logicalDevice, textureImage);

    Sampler sampler(logicalDevice);

    ShaderModule defaultVertShaderModule(logicalDevice, "texture.vert.bin");
    ShaderModule defaultFragShaderModule(logicalDevice, "texture.frag.bin");

    RenderPass renderPass(logicalDevice, swapChain.getFormat().format);

    Pipeline pipeline(
      logicalDevice, defaultVertShaderModule, defaultFragShaderModule, renderPass, swapChain.getExtent());

    VkDescriptorPoolSize uboDescriptorPoolSize{};
    uboDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboDescriptorPoolSize.descriptorCount = static_cast<uint32_t>(swapChainImageViews.get().size());

    VkDescriptorPoolSize samplerDescriptorPoolSize{};
    samplerDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerDescriptorPoolSize.descriptorCount = static_cast<uint32_t>(swapChainImageViews.get().size());

    DescriptorPool descriptorPool(
        logicalDevice, { uboDescriptorPoolSize, samplerDescriptorPoolSize }, static_cast<uint32_t>(swapChainImageViews.get().size()));

    DescriptorSets descriptorSets(logicalDevice,
                                  descriptorPool,
                                  static_cast<uint32_t>(swapChainImageViews.get().size()),
                                  pipeline.getDescriptorSetlayout());

    for(size_t i = 0; i < swapChainImageViews.get().size(); ++i)
    {
        VkDescriptorBufferInfo descriptorBufferinfo{};
        descriptorBufferinfo.buffer = uniformBuffers[i].get();
        descriptorBufferinfo.offset = 0;
        descriptorBufferinfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet writeBufferDescriptorSet{};
        writeBufferDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeBufferDescriptorSet.dstSet = descriptorSets.get().at(i);
        writeBufferDescriptorSet.dstBinding = 0;
        writeBufferDescriptorSet.dstArrayElement = 0;
        writeBufferDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        writeBufferDescriptorSet.descriptorCount = 1;
        writeBufferDescriptorSet.pBufferInfo = &descriptorBufferinfo;
        writeBufferDescriptorSet.pImageInfo = nullptr;
        writeBufferDescriptorSet.pTexelBufferView = nullptr;

        VkDescriptorImageInfo descriptorImageinfo{};
        descriptorImageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        descriptorImageinfo.imageView = textureImageView.get();
        descriptorImageinfo.sampler = sampler.get();

        VkWriteDescriptorSet writeImageDescriptorSet{};
        writeImageDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeImageDescriptorSet.dstSet = descriptorSets.get().at(i);
        writeImageDescriptorSet.dstBinding = 1;
        writeImageDescriptorSet.dstArrayElement = 0;
        writeImageDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        writeImageDescriptorSet.descriptorCount = 1;
        writeImageDescriptorSet.pBufferInfo = nullptr;
        writeImageDescriptorSet.pImageInfo = &descriptorImageinfo;
        writeImageDescriptorSet.pTexelBufferView = nullptr;

        std::array<VkWriteDescriptorSet, 2> writeDescriptorSets{ writeBufferDescriptorSet, writeImageDescriptorSet };
        vkUpdateDescriptorSets(logicalDevice.get(), static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);
    }

    std::vector<FrameBuffer> framebuffers;
    framebuffers.reserve(swapChainImageViews.get().size());

    for(size_t i = 0; i < swapChainImageViews.get().size(); ++i)
    {
        framebuffers.emplace_back(logicalDevice, renderPass, swapChainImageViews.get()[i], swapChain.getExtent());
    }

    CommandPool graphicCommandPool(logicalDevice, VK_QUEUE_GRAPHICS_BIT);

    CommandBuffers graphicCommandBuffers(logicalDevice, graphicCommandPool, swapChainImageViews.get().size());

    for(size_t i = 0; i < swapChainImageViews.get().size(); ++i)
    {
        VkCommandBufferBeginInfo commandBufferBeginInfo{};
        commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        commandBufferBeginInfo.flags = 0;
        commandBufferBeginInfo.pInheritanceInfo = nullptr;

        if(vkBeginCommandBuffer(graphicCommandBuffers.get()[i], &commandBufferBeginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin command buffer");
        }

        VkRenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass = renderPass.get();
        renderPassBeginInfo.framebuffer = framebuffers[i].get();
        renderPassBeginInfo.renderArea.offset = {0, 0};
        renderPassBeginInfo.renderArea.extent = swapChain.getExtent();

        VkClearValue clearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(graphicCommandBuffers.get()[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(graphicCommandBuffers.get()[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.get());

        VkBuffer buffers[] = {vertexBuffer.get()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(graphicCommandBuffers.get()[i], 0, 1, buffers, offsets);

        vkCmdBindIndexBuffer(graphicCommandBuffers.get()[i], indexBuffer.get(), 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(graphicCommandBuffers.get()[i],
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline.getPipelineLayout(),
                                0,
                                1,
                                &descriptorSets.get().at(i),
                                0,
                                nullptr);

        vkCmdDrawIndexed(graphicCommandBuffers.get()[i], 6, 1, 0, 0, 0);

        vkCmdEndRenderPass(graphicCommandBuffers.get()[i]);

        if(vkEndCommandBuffer(graphicCommandBuffers.get()[i]) != VK_SUCCESS)
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

        static auto startTime = std::chrono::high_resolution_clock::now();
        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f),
                                    static_cast<float>(swapChain.getExtent().width) /
                                      static_cast<float>(swapChain.getExtent().height),
                                    0.1f,
                                    10.0f);
        ubo.proj[1][1] *= -1;

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

        uniformDeviceMemories.at(imageIndex).mapMemory(&ubo, uniformBuffers[imageIndex].getSize());

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores.at(currentFrame).get()};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &(graphicCommandBuffers.get()[imageIndex]);

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

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    vkDeviceWaitIdle(logicalDevice.get());

    return EXIT_SUCCESS;
}
