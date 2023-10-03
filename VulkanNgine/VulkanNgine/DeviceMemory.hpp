#pragma once

#include "Buffer.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"
#include "Image.hpp"

class DeviceMemory
{
  public:
    DeviceMemory(const LogicalDevice& _logicalDevice,
                 const PhysicalDevice& _physicalDevice,
                 const Buffer& _buffer,
                 VkMemoryPropertyFlags _properties);

    DeviceMemory(const LogicalDevice& _logicalDevice,
        const PhysicalDevice& _physicalDevice,
        const Image& _image,
        VkMemoryPropertyFlags _properties);

    ~DeviceMemory();

    DeviceMemory(const DeviceMemory&) = delete;

    DeviceMemory(DeviceMemory&& _d);

    DeviceMemory& operator=(const DeviceMemory&) = delete;

    DeviceMemory& operator=(DeviceMemory&&) = delete;

    inline VkDeviceMemory get() const { return m_deviceMemory; }

    void mapMemory(const void* _data, VkDeviceSize _size) const;

  private:
    const LogicalDevice& m_logicalDevice;

    VkDeviceMemory m_deviceMemory;
};
