#pragma once

#include "LogicalDevice.hpp"
#include "SwapChain.hpp"

class SwapChainImageViews
{
  public:
    SwapChainImageViews(const LogicalDevice& _logicalDevice, const SwapChain& _swapChain);

    ~SwapChainImageViews();

    SwapChainImageViews(const SwapChainImageViews&) = delete;

    SwapChainImageViews(SwapChainImageViews&& _f) = delete;

    SwapChainImageViews& operator=(const SwapChainImageViews&) = delete;

    SwapChainImageViews& operator=(SwapChainImageViews&&) = delete;

    inline const std::vector<VkImageView>& get() const { return m_imageViews; }

  private:
    const LogicalDevice& m_logicalDevice;

    std::vector<VkImageView> m_imageViews{};
};
