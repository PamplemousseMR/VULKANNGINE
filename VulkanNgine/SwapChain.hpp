#pragma once

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Surface.hpp"

class SwapChain
{
  public:
    SwapChain(const PhysicalDevice& _physicalDevice, const Surface& _surface, const LogicalDevice& _logicalDevice);

    ~SwapChain();

    SwapChain(const SwapChain&) = delete;

    SwapChain(SwapChain&& _f) = delete;

    SwapChain& operator=(const SwapChain&) = delete;

    SwapChain& operator=(SwapChain&&) = delete;

    inline VkSwapchainKHR get() const { return m_swapChain; }

    inline const std::vector<VkImage>& getImages() const { return m_swapChainImages; }

    inline const VkSurfaceFormatKHR& getFormat() const { return m_surfaceFormat; }

    inline const VkExtent2D getExtent() const { return m_extent; }

  private:
    const LogicalDevice& m_logicalDevice;

    const VkSurfaceFormatKHR m_surfaceFormat;

    const VkExtent2D m_extent;

    VkSwapchainKHR m_swapChain{VK_NULL_HANDLE};

    std::vector<VkImage> m_swapChainImages{};
};
