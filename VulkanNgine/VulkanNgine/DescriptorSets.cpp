#include "DescriptorSets.hpp"

DescriptorSets::DescriptorSets(const LogicalDevice& _logicalDevice,
                               const DescriptorPool& _descriptorPool,
                               uint32_t _count,
                               VkDescriptorSetLayout _descriptorSetLayout)
  : m_logicalDevice(_logicalDevice)
  , m_descriptorPool(_descriptorPool)
{
    m_descriptorSets.resize(_count);

    std::vector<VkDescriptorSetLayout> descriptorSetlayouts(_count, _descriptorSetLayout);
    VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
    descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocInfo.descriptorPool = _descriptorPool.get();
    descriptorSetAllocInfo.descriptorSetCount = _count;
    descriptorSetAllocInfo.pSetLayouts = descriptorSetlayouts.data();

    if(vkAllocateDescriptorSets(_logicalDevice.get(), &descriptorSetAllocInfo, m_descriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor sets");
    }
}

DescriptorSets::~DescriptorSets()
{
    vkFreeDescriptorSets(m_logicalDevice.get(),
                         m_descriptorPool.get(),
                         static_cast<uint32_t>(m_descriptorSets.size()),
                         m_descriptorSets.data());
}
