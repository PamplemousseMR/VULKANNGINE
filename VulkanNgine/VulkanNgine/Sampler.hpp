#pragma once

#include "LogicalDevice.hpp"
#include "Image.hpp"

class Sampler
{
  public:
    Sampler(const LogicalDevice& _logicalDevice, uint32_t _levels);

    ~Sampler();

    Sampler(const Sampler&) = delete;

    Sampler(Sampler&&) = delete;

    Sampler& operator=(const Sampler&) = delete;

    Sampler& operator=(Sampler&&) = delete;

    inline VkSampler get() const { return m_sampler; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkSampler m_sampler{VK_NULL_HANDLE};
};
