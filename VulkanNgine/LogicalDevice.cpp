#include "LogicalDevice.hpp"

#include "logger.hpp"

#include <set>

LogicalDevice::LogicalDevice(const PhysicalDevice& _physicalDevice,
                             VkQueueFlags _queueFlags,
                             bool _surfaceSupport,
                             bool _swapChainSupport)
{
    const std::vector<PhysicalDevice::QueueFamily>::const_iterator graphicQueueFamily =
      std::find_if(_physicalDevice.getQueueFamilies().begin(),
                   _physicalDevice.getQueueFamilies().end(),
                   [&](const PhysicalDevice::QueueFamily& queueFamily) {
                       if(queueFamily.m_graphics)
                       {
                           return true;
                       }
                       return false;
                   });

    if(_queueFlags & VK_QUEUE_GRAPHICS_BIT && graphicQueueFamily == _physicalDevice.getQueueFamilies().end())
    {
        throw std::runtime_error("The device does not have required graphic queue family");
    }

    const std::vector<PhysicalDevice::QueueFamily>::const_iterator computeQueueFamily =
      std::find_if(_physicalDevice.getQueueFamilies().begin(),
                   _physicalDevice.getQueueFamilies().end(),
                   [&](const PhysicalDevice::QueueFamily& queueFamily) {
                       if(queueFamily.m_compute)
                       {
                           return true;
                       }
                       return false;
                   });

    if(_queueFlags & VK_QUEUE_COMPUTE_BIT && computeQueueFamily == _physicalDevice.getQueueFamilies().end())
    {
        throw std::runtime_error("The device does not have required compute queue family");
    }

    const std::vector<PhysicalDevice::QueueFamily>::const_iterator transferQueueFamily =
      std::find_if(_physicalDevice.getQueueFamilies().begin(),
                   _physicalDevice.getQueueFamilies().end(),
                   [&](const PhysicalDevice::QueueFamily& queueFamily) {
                       if(queueFamily.m_transfer)
                       {
                           return true;
                       }
                       return false;
                   });

    if(_queueFlags & VK_QUEUE_TRANSFER_BIT && transferQueueFamily == _physicalDevice.getQueueFamilies().end())
    {
        throw std::runtime_error("The device does not have required transfer queue family");
    }

    const std::vector<PhysicalDevice::QueueFamily>::const_iterator sparseQueueFamily =
      std::find_if(_physicalDevice.getQueueFamilies().begin(),
                   _physicalDevice.getQueueFamilies().end(),
                   [&](const PhysicalDevice::QueueFamily& queueFamily) {
                       if(queueFamily.m_sparseBinding)
                       {
                           return true;
                       }
                       return false;
                   });

    if(_queueFlags & VK_QUEUE_SPARSE_BINDING_BIT && sparseQueueFamily == _physicalDevice.getQueueFamilies().end())
    {
        throw std::runtime_error("The device does not have required sparse queue family");
    }

    const std::vector<PhysicalDevice::QueueFamily>::const_iterator protectQueueFamily =
      std::find_if(_physicalDevice.getQueueFamilies().begin(),
                   _physicalDevice.getQueueFamilies().end(),
                   [&](const PhysicalDevice::QueueFamily& queueFamily) {
                       if(queueFamily.m_protected)
                       {
                           return true;
                       }
                       return false;
                   });

    if(_queueFlags & VK_QUEUE_PROTECTED_BIT && protectQueueFamily == _physicalDevice.getQueueFamilies().end())
    {
        throw std::runtime_error("The device does not have required protected queue family");
    }

    const std::vector<PhysicalDevice::QueueFamily>::const_iterator presentQueueFamily =
      std::find_if(_physicalDevice.getQueueFamilies().begin(),
                   _physicalDevice.getQueueFamilies().end(),
                   [&](const PhysicalDevice::QueueFamily& queueFamily) {
                       if(queueFamily.m_present)
                       {
                           return true;
                       }
                       return false;
                   });

    if(_surfaceSupport && presentQueueFamily == _physicalDevice.getQueueFamilies().end())
    {
        throw std::runtime_error("The device does not have required present queue family");
    }

    if(_swapChainSupport && !_physicalDevice.hasSwapChainSupport())
    {
        throw std::runtime_error("The device does not have swap chain extension");
    }

    std::set<uint32_t> uniqueQueueFamilies{};

    if(graphicQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        uniqueQueueFamilies.insert(graphicQueueFamily->m_index);
        VKNGINE_LOG_VERBOSE("Graphic queue family available at " << graphicQueueFamily->m_index);
    }
    else
    {
        VKNGINE_LOG_VERBOSE("Graphic queue family not available");
    }

    if(computeQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        uniqueQueueFamilies.insert(computeQueueFamily->m_index);
        VKNGINE_LOG_VERBOSE("Compute queue family available at " << computeQueueFamily->m_index);
    }
    else
    {
        VKNGINE_LOG_VERBOSE("Compute queue family not available");
    }

    if(transferQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        uniqueQueueFamilies.insert(transferQueueFamily->m_index);
        VKNGINE_LOG_VERBOSE("Transfer queue family available at " << transferQueueFamily->m_index);
    }
    else
    {
        VKNGINE_LOG_VERBOSE("Transfer queue family not available");
    }

    if(sparseQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        uniqueQueueFamilies.insert(sparseQueueFamily->m_index);
        VKNGINE_LOG_VERBOSE("Sparse queue family available at " << sparseQueueFamily->m_index);
    }
    else
    {
        VKNGINE_LOG_VERBOSE("Sparse queue family not available");
    }

    if(protectQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        uniqueQueueFamilies.insert(protectQueueFamily->m_index);
        VKNGINE_LOG_VERBOSE("Protect queue family available at " << protectQueueFamily->m_index);
    }
    else
    {
        VKNGINE_LOG_VERBOSE("Protect queue family not available");
    }

    if(presentQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        uniqueQueueFamilies.insert(presentQueueFamily->m_index);
        VKNGINE_LOG_VERBOSE("Present queue family available at " << presentQueueFamily->m_index);
    }
    else
    {
        VKNGINE_LOG_VERBOSE("Present queue family not available");
    }

    std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
    for(uint32_t queueFamily : uniqueQueueFamilies)
    {
        VKNGINE_LOG_INFO("Use queue family " << queueFamily);
        VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
        deviceQueueCreateInfo.queueCount = 1;
        deviceQueueCreateInfo.pQueuePriorities = &s_QUEUE_PRIORITY;
        deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
    }
    VkPhysicalDeviceFeatures physicalDeviceFeatures{};

    std::vector<const char*> requiredExtension{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueueCreateInfos.size());
    deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

    if(_physicalDevice.hasSwapChainSupport())
    {
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtension.size());
        deviceCreateInfo.ppEnabledExtensionNames = requiredExtension.data();
    }
    else
    {
        deviceCreateInfo.enabledExtensionCount = 0;
    }

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
        throw std::runtime_error("Failed to create logical device");
    }

    if(graphicQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        VKNGINE_LOG_VERBOSE("Graphic queue family: " << graphicQueueFamily->m_index);
        vkGetDeviceQueue(m_device, graphicQueueFamily->m_index, 0, &m_graphicQueue);
    }

    if(computeQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        VKNGINE_LOG_VERBOSE("Compute queue family: " << computeQueueFamily->m_index);
        vkGetDeviceQueue(m_device, computeQueueFamily->m_index, 0, &m_computeQueue);
    }

    if(transferQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        VKNGINE_LOG_VERBOSE("Transfer queue family: " << transferQueueFamily->m_index);
        vkGetDeviceQueue(m_device, transferQueueFamily->m_index, 0, &m_transferQueue);
    }

    if(sparseQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        VKNGINE_LOG_VERBOSE("Sparse queue family: " << sparseQueueFamily->m_index);
        vkGetDeviceQueue(m_device, sparseQueueFamily->m_index, 0, &m_sparseQueue);
    }

    if(protectQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        VKNGINE_LOG_VERBOSE("Protected queue family: " << protectQueueFamily->m_index);
        vkGetDeviceQueue(m_device, protectQueueFamily->m_index, 0, &m_protectQueue);
    }

    if(presentQueueFamily != _physicalDevice.getQueueFamilies().end())
    {
        VKNGINE_LOG_VERBOSE("Present queue family: " << presentQueueFamily->m_index);
        vkGetDeviceQueue(m_device, presentQueueFamily->m_index, 0, &m_presentQueue);
    }
}

LogicalDevice::~LogicalDevice() { vkDestroyDevice(m_device, nullptr); }
