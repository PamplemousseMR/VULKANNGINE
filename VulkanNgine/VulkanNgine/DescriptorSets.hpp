#pragma once

#include "DescriptorPool.hpp"
#include "LogicalDevice.hpp"

class DescriptorSets
{
  public:
    DescriptorSets(const LogicalDevice& _logicalDevice,
                   const DescriptorPool& _descriptorPool,
                   uint32_t _count,
                   VkDescriptorSetLayout _descriptorSetLayout);

    ~DescriptorSets();

    DescriptorSets(const DescriptorSets&) = delete;

    DescriptorSets(DescriptorSets&& _f) = delete;

    DescriptorSets& operator=(const DescriptorSets&) = delete;

    DescriptorSets& operator=(DescriptorSets&&) = delete;

    inline std::vector<VkDescriptorSet> get() const { return m_descriptorSets; }

  private:
    const LogicalDevice& m_logicalDevice;

    const DescriptorPool& m_descriptorPool;

    std::vector<VkDescriptorSet> m_descriptorSets;
};
