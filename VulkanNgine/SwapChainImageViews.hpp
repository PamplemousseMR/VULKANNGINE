#pragma once

#include "LogicalDevice.hpp"
#include "SwapChain.hpp"

class SwapChainImageViews
{
  public:
    SwapChainImageViews(const LogicalDevice& _logicalDevice, const SwapChain& _swapChain);

    ~SwapChainImageViews();

  private:
    const LogicalDevice& m_logicalDevice;

    std::vector<VkImageView> m_imageViews{};
};
