#pragma once

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"

class SwapChain
{
  public:
    SwapChain(const PhysicalDevice& _physicalDevice, const Surface& _surface, const LogicalDevice& _logicalDevice);

    ~SwapChain();

  private:
    const LogicalDevice& m_logicalDevice;

    const VkSurfaceFormatKHR m_surfaceFormat;

    const VkExtent2D m_extent;

    VkSwapchainKHR m_swapChain{VK_NULL_HANDLE};

    std::vector<VkImage> m_swapChainImages{};
};
