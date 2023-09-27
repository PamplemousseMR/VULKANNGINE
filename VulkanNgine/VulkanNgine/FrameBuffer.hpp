#pragma once

#include "LogicalDevice.hpp"
#include "RenderPass.hpp"
#include "SwapChain.hpp"
#include "SwapChainImageViews.hpp"

#include <iostream>

class FrameBuffer
{
  public:
    FrameBuffer(const LogicalDevice& _logicalDevice,
                const RenderPass& _renderPass,
                VkImageView _imageView,
                VkExtent2D _size);

    ~FrameBuffer();

    FrameBuffer(const FrameBuffer&) = delete;

    FrameBuffer(FrameBuffer&& _f);

    FrameBuffer& operator=(const FrameBuffer&) = delete;

    FrameBuffer& operator=(FrameBuffer&&) = delete;

    inline VkFramebuffer get() const { return m_framebuffer; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkFramebuffer m_framebuffer{VK_NULL_HANDLE};
};
