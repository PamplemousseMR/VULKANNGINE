#include "CommandPool.hpp"

CommandPool::CommandPool(const LogicalDevice& _logicalDevice, VkQueueFlagBits _queueFlag)
  : m_logicalDevice(_logicalDevice)
{
    VkCommandPoolCreateInfo commandPoolCreateInfo{};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    switch(_queueFlag)
    {
        case VK_QUEUE_GRAPHICS_BIT: commandPoolCreateInfo.queueFamilyIndex = _logicalDevice.getGraphicQueueIdx(); break;
        case VK_QUEUE_COMPUTE_BIT: commandPoolCreateInfo.queueFamilyIndex = _logicalDevice.getComputeQueueIdx(); break;
        case VK_QUEUE_TRANSFER_BIT:
            commandPoolCreateInfo.queueFamilyIndex = _logicalDevice.getTransferQueueIdx();
            break;
        case VK_QUEUE_SPARSE_BINDING_BIT:
            commandPoolCreateInfo.queueFamilyIndex = _logicalDevice.getSparseQueueIdx();
            break;
        case VK_QUEUE_PROTECTED_BIT:
            commandPoolCreateInfo.queueFamilyIndex = _logicalDevice.getProtectQueueIdx();
            break;
        default: throw std::invalid_argument("Unhandle queue flag bit");
    }
    commandPoolCreateInfo.flags = 0;

    if(vkCreateCommandPool(_logicalDevice.get(), &commandPoolCreateInfo, nullptr, &m_commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create command pool");
    }
}

CommandPool::~CommandPool() { vkDestroyCommandPool(m_logicalDevice.get(), m_commandPool, nullptr); }
