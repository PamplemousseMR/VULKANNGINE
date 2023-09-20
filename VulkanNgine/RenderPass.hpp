#pragma once

#include "LogicalDevice.hpp"
#include "SwapChain.hpp"

class RenderPass
{
  public:
    RenderPass(const LogicalDevice& _logicalDevice, const SwapChain& _swapChain);

    ~RenderPass();

    inline VkRenderPass get() const { return m_renderPass; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkRenderPass m_renderPass{VK_NULL_HANDLE};
};
