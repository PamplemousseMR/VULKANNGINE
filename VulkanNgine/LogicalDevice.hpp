#pragma once

#include "PhysicalDevice.hpp"

class LogicalDevice
{
  public:
    LogicalDevice(const PhysicalDevice& _physicalDevice, VkQueueFlags _queueFlags);

    ~LogicalDevice();

  private:
    static constexpr float s_QUEUE_PRIORITY{1.0f};

    VkDevice m_device{VK_NULL_HANDLE};
};
