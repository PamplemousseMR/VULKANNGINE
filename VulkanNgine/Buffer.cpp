#include "Buffer.hpp"

Buffer::Buffer(const LogicalDevice& _logicalDevice, VkBufferUsageFlags _usage, const std::vector<Buffer::Vertex>& _data)
  : m_logicalDevice(_logicalDevice)
  , m_data(_data)
{
    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = sizeof(_data[0]) * _data.size();
    bufferCreateInfo.usage = _usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if(vkCreateBuffer(_logicalDevice.get(), &bufferCreateInfo, nullptr, &m_buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer");
    }

    vkGetBufferMemoryRequirements(_logicalDevice.get(), m_buffer, &m_requirements);
}

Buffer::~Buffer() { vkDestroyBuffer(m_logicalDevice.get(), m_buffer, nullptr); }
