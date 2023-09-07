#include "LogicalDevice.hpp"

#include "logger.hpp"

LogicalDevice::LogicalDevice(const PhysicalDevice& _physicalDevice, VkQueueFlags _queueFlags)
{
    const auto selectedQueueFamily =
      std::find_if(_physicalDevice.getQueueFamilies().begin(),
                   _physicalDevice.getQueueFamilies().end(),
                   [&](const PhysicalDevice::QueueFamily& queueFamily) {
                       if(_queueFlags & VK_QUEUE_GRAPHICS_BIT && !queueFamily.m_graphics)
                       {
                           return false;
                       }
                       if(_queueFlags & VK_QUEUE_COMPUTE_BIT && !queueFamily.m_compute)
                       {
                           return false;
                       }
                       if(_queueFlags & VK_QUEUE_TRANSFER_BIT && !queueFamily.m_transfer)
                       {
                           return false;
                       }
                       if(_queueFlags & VK_QUEUE_SPARSE_BINDING_BIT && !queueFamily.m_sparseBinding)
                       {
                           return false;
                       }
                       if(_queueFlags & VK_QUEUE_PROTECTED_BIT && !queueFamily.m_protected)
                       {
                           return false;
                       }
                       return true;
                   });

    if(selectedQueueFamily == _physicalDevice.getQueueFamilies().end())
    {
        throw std::runtime_error("The device does not have required queue family");
    }

    VKNGINE_LOG_VERBOSE("Selected queue family: " << selectedQueueFamily->m_index);

    VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.queueFamilyIndex = selectedQueueFamily->m_index;
    deviceQueueCreateInfo.queueCount = 1;
    deviceQueueCreateInfo.pQueuePriorities = &s_QUEUE_PRIORITY;

    VkPhysicalDeviceFeatures physicalDeviceFeatures{};

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
    deviceCreateInfo.enabledExtensionCount = 0;
    if(Instance::s_ENABLE_VALIDATION_LAYERS)
    {
        deviceCreateInfo.ppEnabledLayerNames = Instance::s_VALIDATION_LAYERS.data();
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(Instance::s_VALIDATION_LAYERS.size());
    }
    else
    {
        deviceCreateInfo.enabledLayerCount = 0;
    }

    if(vkCreateDevice(_physicalDevice.get(), &deviceCreateInfo, nullptr, &m_device) != VK_SUCCESS)
    {
        throw std::runtime_error("Échec de création du périphérique logique !");
    }
}

LogicalDevice::~LogicalDevice() { vkDestroyDevice(m_device, nullptr); }
