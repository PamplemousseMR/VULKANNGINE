#include "DeviceMemory.hpp"

DeviceMemory::DeviceMemory(const LogicalDevice& _logicalDevice,
                           const PhysicalDevice& _physicalDevice,
                           const Buffer& _buffer,
                           VkMemoryPropertyFlags _properties)
  : m_logicalDevice(_logicalDevice)
  , m_buffer(_buffer)
{
    VkMemoryRequirements memoryRequirements = _buffer.getRequirements();
    VkMemoryAllocateInfo memoryAllocationInfo{};
    memoryAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocationInfo.allocationSize = memoryRequirements.size;
    memoryAllocationInfo.memoryTypeIndex =
      _physicalDevice.findMemoryType(memoryRequirements.memoryTypeBits, _properties);

    if(vkAllocateMemory(_logicalDevice.get(), &memoryAllocationInfo, nullptr, &m_deviceMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create device memory");
    }

    vkBindBufferMemory(_logicalDevice.get(), _buffer.get(), m_deviceMemory, 0);
}

DeviceMemory::~DeviceMemory() { vkFreeMemory(m_logicalDevice.get(), m_deviceMemory, nullptr); }

void DeviceMemory::mapMemory() const
{
    void* data;
    vkMapMemory(m_logicalDevice.get(), m_deviceMemory, 0, m_buffer.getSize(), 0, &data);
    memcpy(data, m_buffer.getData().data(), m_buffer.getSize());
    vkUnmapMemory(m_logicalDevice.get(), m_deviceMemory);
}
