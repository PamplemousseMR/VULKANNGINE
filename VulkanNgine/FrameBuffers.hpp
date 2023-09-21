#pragma once

#include "LogicalDevice.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"
#include "SwapChainImageViews.hpp"

class FrameBuffers
{
  public:
    FrameBuffers(const LogicalDevice& _logicalDevice,
                 const SwapChain& _swapChain,
                 const SwapChainImageViews& _swapChainImageViews,
                 const RenderPass& _renderPass);

    ~FrameBuffers();

  private:
    const LogicalDevice& m_logicalDevice;

    std::vector<VkFramebuffer> m_framebuffers;
};
