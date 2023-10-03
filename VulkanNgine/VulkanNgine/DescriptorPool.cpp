#include "DescriptorPool.hpp"

DescriptorPool::DescriptorPool(const LogicalDevice& _logicalDevice, std::vector<VkDescriptorPoolSize> _poolSizes, uint32_t _count)
  : m_logicalDevice(_logicalDevice)
{
    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(_poolSizes.size());;
    descriptorPoolCreateInfo.pPoolSizes = _poolSizes.data();
    descriptorPoolCreateInfo.maxSets = _count;
    descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

    if(vkCreateDescriptorPool(_logicalDevice.get(), &descriptorPoolCreateInfo, nullptr, &m_descriptorPool) !=
       VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }
}

DescriptorPool::~DescriptorPool() { vkDestroyDescriptorPool(m_logicalDevice.get(), m_descriptorPool, nullptr); }
