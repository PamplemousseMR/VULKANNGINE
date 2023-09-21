#pragma once

#include "LogicalDevice.hpp"

class Semaphore
{
  public:
    Semaphore(const LogicalDevice& _logicalDevice);

    ~Semaphore();

    Semaphore(const Semaphore&) = delete;

    Semaphore(Semaphore&&) = delete;

    Semaphore& operator=(const Semaphore&) = delete;

    Semaphore& operator=(Semaphore&&) = delete;

    inline VkSemaphore get() const { return m_semaphore; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkSemaphore m_semaphore;
};
