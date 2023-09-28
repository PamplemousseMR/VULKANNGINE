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

Buffer::~Buffer()
{
    if(m_buffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(m_logicalDevice.get(), m_buffer, nullptr);
    }
}

Buffer::Buffer(Buffer&& _b)
  : m_logicalDevice(std::move(_b.m_logicalDevice))
  , m_size(std::move(_b.m_size))
  , m_buffer(std::exchange(_b.m_buffer, VK_NULL_HANDLE))
  , m_requirements(std::move(_b.m_requirements))
{}
