#include "CommandBuffers.hpp"

CommandBuffers::CommandBuffers(const LogicalDevice& _logicalDevice, const CommandPool& _commandPool, size_t count)
  : m_logicalDevice(_logicalDevice)
  , m_commandPool(_commandPool)
{
    m_commandBuffers.resize(count);

    VkCommandBufferAllocateInfo commandBufferAllocInfo{};
    commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocInfo.commandPool = _commandPool.get();
    commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocInfo.commandBufferCount = static_cast<uint32_t>(count);

    if(vkAllocateCommandBuffers(_logicalDevice.get(), &commandBufferAllocInfo, m_commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffer");
    }
}

CommandBuffers::~CommandBuffers()
{
    vkFreeCommandBuffers(m_logicalDevice.get(),
                         m_commandPool.get(),
                         static_cast<uint32_t>(m_commandBuffers.size()),
                         m_commandBuffers.data());
}
