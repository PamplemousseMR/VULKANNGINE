#include "Instance.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Pipeline.hpp"
#include "RenderPass.hpp"
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

    SwapChainImageViews swapChainimageViews(logicalDevice, swapChain);

    ShaderModule defaultVertShaderModule(logicalDevice, "Shaders/default.vert.bin");
    ShaderModule defaultFragShaderModule(logicalDevice, "Shaders/default.frag.bin");

    RenderPass renderPass(logicalDevice, swapChain);
    Pipeline pipeline(logicalDevice, defaultVertShaderModule, defaultFragShaderModule, renderPass, swapChain);

    window.run();

    return EXIT_SUCCESS;
}
