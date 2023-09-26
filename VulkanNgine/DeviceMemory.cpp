#include "DeviceMemory.hpp"

DeviceMemory::DeviceMemory(const LogicalDevice& _logicalDevice,
                           const PhysicalDevice& _physicalDevice,
                           const Buffer& _buffer)
  : m_logicalDevice(_logicalDevice)
{
    VkMemoryRequirements memoryRequirements = _buffer.getRequirements();
    VkMemoryAllocateInfo memoryAllocationInfo{};
    memoryAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocationInfo.allocationSize = memoryRequirements.size;
    memoryAllocationInfo.memoryTypeIndex = _physicalDevice.findMemoryType(
      memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if(vkAllocateMemory(_logicalDevice.get(), &memoryAllocationInfo, nullptr, &m_deviceMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create device memory");
    }

    vkBindBufferMemory(_logicalDevice.get(), _buffer.get(), m_deviceMemory, 0);

    void* data;
    vkMapMemory(_logicalDevice.get(), m_deviceMemory, 0, _buffer.getSize(), 0, &data);
    memcpy(data, _buffer.getData().data(), _buffer.getSize());
    vkUnmapMemory(_logicalDevice.get(), m_deviceMemory);
}

DeviceMemory::~DeviceMemory() { vkFreeMemory(m_logicalDevice.get(), m_deviceMemory, nullptr); }
