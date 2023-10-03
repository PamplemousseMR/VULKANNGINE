#pragma once

#include "LogicalDevice.hpp"
#include "SwapChain.hpp"

class RenderPass
{
  public:
    RenderPass(const LogicalDevice& _logicalDevice, VkFormat _format);

    RenderPass(const LogicalDevice& _logicalDevice, VkFormat _colorFormat, VkFormat _depthFormat);

    ~RenderPass();

    RenderPass(const RenderPass&) = delete;

    RenderPass(RenderPass&& _f) = delete;

    RenderPass& operator=(const RenderPass&) = delete;

    RenderPass& operator=(RenderPass&&) = delete;

    inline VkRenderPass get() const { return m_renderPass; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkRenderPass m_renderPass{VK_NULL_HANDLE};
};
