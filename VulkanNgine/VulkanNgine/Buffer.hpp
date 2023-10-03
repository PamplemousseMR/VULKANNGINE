#pragma once

#include "LogicalDevice.hpp"

class Buffer
{
  public:
    Buffer(const LogicalDevice& _logicalDevice, VkBufferUsageFlags _usage, VkDeviceSize _size);

    ~Buffer();

    Buffer(const Buffer&) = delete;

    Buffer(Buffer&& _b);

    Buffer& operator=(const Buffer&) = delete;

    Buffer& operator=(Buffer&&) = delete;

    inline VkBuffer get() const { return m_buffer; }

    inline VkMemoryRequirements getRequirements() const { return m_requirements; }

    inline VkDeviceSize getSize() const { return m_size; }

  private:
    const LogicalDevice& m_logicalDevice;

    const VkDeviceSize m_size;

    VkBuffer m_buffer;

    VkMemoryRequirements m_requirements;
};
