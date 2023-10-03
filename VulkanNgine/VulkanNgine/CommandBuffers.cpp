#include "CommandBuffers.hpp"

CommandBuffers::CommandBuffers(const LogicalDevice& _logicalDevice, const CommandPool& _commandPool, size_t _count)
  : m_logicalDevice(_logicalDevice)
  , m_commandPool(_commandPool)
{
    m_commandBuffers.resize(_count);

    VkCommandBufferAllocateInfo commandBufferAllocInfo{};
    commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocInfo.commandPool = _commandPool.get();
    commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocInfo.commandBufferCount = static_cast<uint32_t>(_count);

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

void CommandBuffers::beginSingleTimeCommand(size_t _index) const
{
    VkCommandBufferBeginInfo commandBufferBeginInfo{};
    commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(m_commandBuffers[_index], &commandBufferBeginInfo);
}

void CommandBuffers::endSingleTimeCommand(size_t _index) const
{
    vkEndCommandBuffer(m_commandBuffers[_index]);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers[_index];

    vkQueueSubmit(m_logicalDevice.getTransferQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_logicalDevice.getTransferQueue());
}