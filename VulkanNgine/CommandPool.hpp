#pragma once

#include "LogicalDevice.hpp"

class CommandPool
{
  public:
    CommandPool(const LogicalDevice& _logicalDevice, VkQueueFlagBits _queueFlag);

    ~CommandPool();

    CommandPool(const CommandPool&) = delete;

    CommandPool(CommandPool&& _f) = delete;

    CommandPool& operator=(const CommandPool&) = delete;

    CommandPool& operator=(CommandPool&&) = delete;

    inline VkCommandPool get() const { return m_commandPool; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkCommandPool m_commandPool{VK_NULL_HANDLE};
};
