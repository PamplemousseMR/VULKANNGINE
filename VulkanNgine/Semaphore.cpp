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

Semaphore::~Semaphore() { vkDestroySemaphore(m_logicalDevice.get(), m_semaphore, nullptr); }
