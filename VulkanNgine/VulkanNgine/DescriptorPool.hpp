#pragma once

#include "LogicalDevice.hpp"

class DescriptorPool
{
  public:
    DescriptorPool(const LogicalDevice& _logicalDevice, std::vector<VkDescriptorPoolSize> _poolSizes, uint32_t _count);

    ~DescriptorPool();

    DescriptorPool(const DescriptorPool&) = delete;

    DescriptorPool(DescriptorPool&&) = delete;

    DescriptorPool& operator=(const DescriptorPool&) = delete;

    DescriptorPool& operator=(DescriptorPool&&) = delete;

    inline VkDescriptorPool get() const { return m_descriptorPool; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkDescriptorPool m_descriptorPool;
};
