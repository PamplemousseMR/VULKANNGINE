#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer(const LogicalDevice& _logicalDevice,
                         const RenderPass& _renderPass,
                         VkImageView _imageView,
                         VkExtent2D _size)
  : m_logicalDevice(_logicalDevice)
{
    VkImageView imageViews[] = {_imageView};

    VkFramebufferCreateInfo framebufferCreateInfo{};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = _renderPass.get();
    framebufferCreateInfo.attachmentCount = 1;
    framebufferCreateInfo.pAttachments = imageViews;
    framebufferCreateInfo.width = _size.width;
    framebufferCreateInfo.height = _size.height;
    framebufferCreateInfo.layers = 1;

    if(vkCreateFramebuffer(_logicalDevice.get(), &framebufferCreateInfo, nullptr, &m_framebuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create framebuffer");
    }
}

FrameBuffer::~FrameBuffer()
{
    if(m_framebuffer != VK_NULL_HANDLE)
    {
        vkDestroyFramebuffer(m_logicalDevice.get(), m_framebuffer, nullptr);
    }
}

FrameBuffer::FrameBuffer(FrameBuffer&& _f)
  : m_logicalDevice(std::move(_f.m_logicalDevice))
  , m_framebuffer(std::exchange(_f.m_framebuffer, VK_NULL_HANDLE))
{}
