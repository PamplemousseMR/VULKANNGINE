#include "PhysicalDevice.hpp"

#include "logger.hpp"

namespace {

bool checkExtensionSupport(VkPhysicalDevice _device, const std::vector<const char*>& _extensions)
{
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(_device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(_device, nullptr, &extensionCount, availableExtensions.data());

    for(const char* extensionName : _extensions)
    {
        bool extensionFound = false;

        for(const auto& ex : availableExtensions)
        {
            if(strcmp(extensionName, ex.extensionName) == 0)
            {
                extensionFound = true;
                break;
            }
        }

        if(!extensionFound)
        {
            VKNGINE_LOG_WARNING("Extension '" << extensionName << "' not available");
            return false;
        }
    }

    return true;
}

}
std::vector<PhysicalDevice> PhysicalDevice::getDevices(const Instance& _instance, const Surface& _surface)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance.get(), &deviceCount, nullptr);

    if(deviceCount == 0)
    {
        throw std::runtime_error("No device found");
    }

    std::vector<VkPhysicalDevice> availableDevices(deviceCount);
    vkEnumeratePhysicalDevices(_instance.get(), &deviceCount, availableDevices.data());

    std::vector<PhysicalDevice> devices;

    for(const VkPhysicalDevice& device : availableDevices)
    {
        VkPhysicalDeviceProperties physicalDeviceProperties;
        vkGetPhysicalDeviceProperties(device, &physicalDeviceProperties);

        VKNGINE_LOG_INFO("Available device: " << physicalDeviceProperties.deviceName);

        // Check queue families
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> availableQueueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, availableQueueFamilies.data());

        std::vector<QueueFamily> queueFamilies;

        for(uint32_t i = 0; i < queueFamilyCount; ++i)
        {
            QueueFamily queueFamily{i};

            queueFamily.m_count = availableQueueFamilies[i].queueCount;

            if(availableQueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                queueFamily.m_graphics = true;
            }
            if(availableQueueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                queueFamily.m_compute = true;
            }
            if(availableQueueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                queueFamily.m_transfer = true;
            }
            if(availableQueueFamilies[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
            {
                queueFamily.m_sparseBinding = true;
            }
            if(availableQueueFamilies[i].queueFlags & VK_QUEUE_PROTECTED_BIT)
            {
                queueFamily.m_protected = true;
            }

            // Check surface support
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface.get(), &presentSupport);
            if(presentSupport)
            {
                queueFamily.m_present = true;
            }

            queueFamilies.push_back(queueFamily);
        }

        const bool swapChainSupport = checkExtensionSupport(device, {VK_KHR_SWAPCHAIN_EXTENSION_NAME});

        SwapChainSupportDetails details;

        if(swapChainSupport)
        {
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface.get(), &details.m_capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface.get(), &formatCount, nullptr);
            if(formatCount != 0)
            {
                details.m_formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface.get(), &formatCount, details.m_formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface.get(), &presentModeCount, nullptr);

            if(presentModeCount != 0)
            {
                details.m_presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(
                  device, _surface.get(), &presentModeCount, details.m_presentModes.data());
            }
        }

        devices.push_back(
          PhysicalDevice(device, physicalDeviceProperties.deviceName, queueFamilies, swapChainSupport, details));
    }

    return devices;
}

PhysicalDevice::PhysicalDevice(VkPhysicalDevice _physicalDevice,
                               const std::string& _name,
                               const std::vector<QueueFamily>& _queueFamilies,
                               bool _swapChainSupport,
                               const SwapChainSupportDetails _swapChainSupportDetails)
  : m_physicalDevice(_physicalDevice)
  , m_name(_name)
  , m_queueFamilies(_queueFamilies)
  , m_swapChainSupport(_swapChainSupport)
  , m_swapChainSupportDetails(_swapChainSupportDetails)
{}
