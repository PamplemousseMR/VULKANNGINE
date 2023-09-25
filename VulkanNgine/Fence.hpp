#pragma once

#include "LogicalDevice.hpp"

class Fence
{
  public:
    Fence(const LogicalDevice& _logicalDevice);

    ~Fence();

    Fence(const Fence&) = delete;

    Fence(Fence&& _s);

    Fence& operator=(const Fence&) = delete;

    Fence& operator=(Fence&&) = delete;

    inline VkFence get() const { return m_fence; }

    void wait();

    void reset();

  private:
    const LogicalDevice& m_logicalDevice;

    VkFence m_fence{VK_NULL_HANDLE};
};
