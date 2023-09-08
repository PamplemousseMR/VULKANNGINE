#include "Instance.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"
#include "Window.hpp"
#include "logger.hpp"

#include <cstdlib>
#include <iostream>

int main()
{
    Window window("VulkanNgine");

    Instance instance(window);

    Surface surface(window, instance);

    PhysicalDevice physicalDevice = PhysicalDevice::getDevices(instance)[0];
    VKNGINE_LOG_VERBOSE("Selected physical device " << physicalDevice.getName());
    for(PhysicalDevice::QueueFamily queueFamily : physicalDevice.getQueueFamilies())
    {
        VKNGINE_LOG_VERBOSE("   Queue family " << queueFamily.m_index);
        VKNGINE_LOG_VERBOSE("       Graphics " << queueFamily.m_graphics);
        VKNGINE_LOG_VERBOSE("       Compute " << queueFamily.m_compute);
        VKNGINE_LOG_VERBOSE("       Transfer " << queueFamily.m_transfer);
        VKNGINE_LOG_VERBOSE("       Sparse " << queueFamily.m_sparseBinding);
        VKNGINE_LOG_VERBOSE("       Protected " << queueFamily.m_protected);
    }

    LogicalDevice logicalDevice(physicalDevice, VK_QUEUE_GRAPHICS_BIT);

    window.run();

    return EXIT_SUCCESS;
}
