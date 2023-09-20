#pragma once

#include "LogicalDevice.hpp"
#include "SwapChain.hpp"

class SwapChainImageViews
{
  public:
    SwapChainImageViews(const SwapChain& _swapChain, const LogicalDevice& _logicalDevice);

    ~SwapChainImageViews();

  private:
    const LogicalDevice& m_logicalDevice;

    std::vector<VkImageView> m_imageViews{};
};
