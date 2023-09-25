#include "Semaphore.hpp"

Semaphore::Semaphore(const LogicalDevice& _logicalDevice)
  : m_logicalDevice(_logicalDevice)
{
    VkSemaphoreCreateInfo semaphoreCreateInfo{};
    semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if(vkCreateSemaphore(_logicalDevice.get(), &semaphoreCreateInfo, nullptr, &m_semaphore) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create semaphore");
    }
}

Semaphore::~Semaphore()
{
    if(m_semaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(m_logicalDevice.get(), m_semaphore, nullptr);
    }
}

Semaphore::Semaphore(Semaphore&& _s)
  : m_logicalDevice(std::move(_s.m_logicalDevice))
  , m_semaphore(std::exchange(_s.m_semaphore, VK_NULL_HANDLE))
{}
