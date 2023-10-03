#include "FrameBuffer.hpp"

FrameBuffer::FrameBuffer(const LogicalDevice& _logicalDevice,
                         const RenderPass& _renderPass,
                            std::vector<VkImageView> _imageViews,
                         VkExtent2D _size)
  : m_logicalDevice(_logicalDevice)
{
    VkFramebufferCreateInfo framebufferCreateInfo{};
    framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferCreateInfo.renderPass = _renderPass.get();
    framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(_imageViews.size());;
    framebufferCreateInfo.pAttachments = _imageViews.data();
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
