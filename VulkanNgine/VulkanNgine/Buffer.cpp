#include "Buffer.hpp"

Buffer::Buffer(const LogicalDevice& _logicalDevice, VkBufferUsageFlags _usage, VkDeviceSize _size)
  : m_logicalDevice(_logicalDevice)
  , m_size(_size)
{
    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = _size;
    bufferCreateInfo.usage = _usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(vkCreateBuffer(_logicalDevice.get(), &bufferCreateInfo, nullptr, &m_buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer");
    }

    vkGetBufferMemoryRequirements(_logicalDevice.get(), m_buffer, &m_requirements);
}

Buffer::~Buffer() { vkDestroyBuffer(m_logicalDevice.get(), m_buffer, nullptr); }
