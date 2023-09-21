#include "FrameBuffers.hpp"

FrameBuffers::FrameBuffers(const LogicalDevice& _logicalDevice,
                           const SwapChain& _swapChain,
                           const SwapChainImageViews& _swapChainImageViews,
                           const RenderPass& _renderPass)
  : m_logicalDevice(_logicalDevice)
{
    m_framebuffers.resize(_swapChainImageViews.get().size());
    for(size_t i = 0; i < _swapChainImageViews.get().size(); ++i)
    {
        VkImageView imageView[] = {_swapChainImageViews.get()[i]};

        VkFramebufferCreateInfo framebufferCreateInfo{};
        framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferCreateInfo.renderPass = _renderPass.get();
        framebufferCreateInfo.attachmentCount = 1;
        framebufferCreateInfo.pAttachments = imageView;
        framebufferCreateInfo.width = _swapChain.getExtent().width;
        framebufferCreateInfo.height = _swapChain.getExtent().height;
        framebufferCreateInfo.layers = 1;

        if(vkCreateFramebuffer(_logicalDevice.get(), &framebufferCreateInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create framebuffer");
        }
    }
}

FrameBuffers::~FrameBuffers()
{
    for(VkFramebuffer& framebuffer : m_framebuffers)
    {
        vkDestroyFramebuffer(m_logicalDevice.get(), framebuffer, nullptr);
    }
}
