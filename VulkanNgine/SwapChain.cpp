#include "SwapChain.hpp"

#include "Window.hpp"

namespace {

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for(const auto& availableFormat : availableFormats)
    {
        if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
           availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for(const auto& availablePresentMode : availablePresentModes)
    {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
    if(capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    else
    {
        VkExtent2D actualExtent = {Window::s_WIDTH, Window::s_HEIGHT};

        actualExtent.width =
          std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height,
                                       std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

}

SwapChain::SwapChain(const PhysicalDevice& _physicalDevice,
                     const Surface& _surface,
                     const LogicalDevice& _logicalDevice)
  : m_logicalDevice(_logicalDevice)
{
    const PhysicalDevice::SwapChainSupportDetails details = _physicalDevice.getSwapChainSupportDetails();

    const VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(details.m_formats);
    const VkPresentModeKHR presentMode = chooseSwapPresentMode(details.m_presentModes);
    const VkExtent2D extent = chooseSwapExtent(details.m_capabilities);

    uint32_t imageCount = details.m_capabilities.minImageCount + 1;
    if(details.m_capabilities.maxImageCount > 0 && imageCount > details.m_capabilities.maxImageCount)
    {
        imageCount = details.m_capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapChainCreateInfo{};
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = _surface.get();
    swapChainCreateInfo.minImageCount = imageCount;
    swapChainCreateInfo.imageFormat = surfaceFormat.format;
    swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainCreateInfo.imageExtent = extent;
    swapChainCreateInfo.imageArrayLayers = 1;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {_logicalDevice.getgraphicQueueIdx(), _logicalDevice.getpresentQueueIdx()};
    if(_logicalDevice.getgraphicQueueIdx() != _logicalDevice.getpresentQueueIdx())
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainCreateInfo.queueFamilyIndexCount = 0;     // Optionnel
        swapChainCreateInfo.pQueueFamilyIndices = nullptr; // Optionnel
    }

    swapChainCreateInfo.preTransform = details.m_capabilities.currentTransform;
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    if(vkCreateSwapchainKHR(_logicalDevice.get(), &swapChainCreateInfo, nullptr, &m_swapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create swap chain");
    }
}

SwapChain::~SwapChain() { vkDestroySwapchainKHR(m_logicalDevice.get(), m_swapChain, nullptr); }
