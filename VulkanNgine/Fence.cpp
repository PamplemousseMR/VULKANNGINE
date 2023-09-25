#include "Fence.hpp"

Fence::Fence(const LogicalDevice& _logicalDevice)
  : m_logicalDevice(_logicalDevice)
{
    VkFenceCreateInfo fenceCreateInfo{};
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if(vkCreateFence(_logicalDevice.get(), &fenceCreateInfo, nullptr, &m_fence) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create fence");
    }
}

Fence::~Fence()
{
    if(m_fence != VK_NULL_HANDLE)
    {
        vkDestroyFence(m_logicalDevice.get(), m_fence, nullptr);
    }
}

Fence::Fence(Fence&& _s)
  : m_logicalDevice(std::move(_s.m_logicalDevice))
  , m_fence(std::move(_s.m_fence))
{}

void Fence::wait() { vkWaitForFences(m_logicalDevice.get(), 1, &m_fence, VK_TRUE, UINT64_MAX); }

void Fence::reset() { vkResetFences(m_logicalDevice.get(), 1, &m_fence); }
