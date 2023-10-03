#include "DeviceMemory.hpp"

DeviceMemory::DeviceMemory(const LogicalDevice& _logicalDevice,
                           const PhysicalDevice& _physicalDevice,
                           const Buffer& _buffer,
                           VkMemoryPropertyFlags _properties)
  : m_logicalDevice(_logicalDevice)
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

DeviceMemory::DeviceMemory(const LogicalDevice& _logicalDevice,
    const PhysicalDevice& _physicalDevice,
    const Image& _image,
    VkMemoryPropertyFlags _properties)
    : m_logicalDevice(_logicalDevice)
{
    VkMemoryRequirements memoryRequirements = _image.getRequirements();
    VkMemoryAllocateInfo memoryAllocationInfo{};
    memoryAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocationInfo.allocationSize = memoryRequirements.size;
    memoryAllocationInfo.memoryTypeIndex =
        _physicalDevice.findMemoryType(memoryRequirements.memoryTypeBits, _properties);

    if (vkAllocateMemory(_logicalDevice.get(), &memoryAllocationInfo, nullptr, &m_deviceMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create device memory");
    }

    vkBindImageMemory(_logicalDevice.get(), _image.get(), m_deviceMemory, 0);
}

DeviceMemory::~DeviceMemory()
{
    if(m_deviceMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(m_logicalDevice.get(), m_deviceMemory, nullptr);
    }
}

void DeviceMemory::mapMemory(const void* _data, VkDeviceSize _size) const
{
    void* data;
    vkMapMemory(m_logicalDevice.get(), m_deviceMemory, 0, _size, 0, &data);
    memcpy(data, _data, _size);
    vkUnmapMemory(m_logicalDevice.get(), m_deviceMemory);
}

DeviceMemory::DeviceMemory(DeviceMemory&& _d)
  : m_logicalDevice(std::move(_d.m_logicalDevice))
  , m_deviceMemory(std::exchange(_d.m_deviceMemory, VK_NULL_HANDLE))
{}
