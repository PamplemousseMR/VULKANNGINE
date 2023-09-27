#pragma once

#include "Buffer.hpp"
#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"

class DeviceMemory
{
  public:
    DeviceMemory(const LogicalDevice& _logicalDevice, const PhysicalDevice& _physicalDevice, const Buffer& _buffer, VkMemoryPropertyFlags _properties);

    ~DeviceMemory();

    DeviceMemory(const DeviceMemory&) = delete;

    DeviceMemory(DeviceMemory&& _f) = delete;

    DeviceMemory& operator=(const DeviceMemory&) = delete;

    DeviceMemory& operator=(DeviceMemory&&) = delete;

    inline VkDeviceMemory get() const { return m_deviceMemory; }

    void mapMemory() const;

  private:
    const LogicalDevice& m_logicalDevice;

    const Buffer& m_buffer;

    VkDeviceMemory m_deviceMemory;
};
