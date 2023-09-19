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

    VkSwapchainKHR m_swapChain{VK_NULL_HANDLE};
};
