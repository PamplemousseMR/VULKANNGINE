#pragma once

#include "CommandPool.hpp"
#include "LogicalDevice.hpp"

class CommandBuffers
{
  public:
    CommandBuffers(const LogicalDevice& _logicalDevice, const CommandPool& _commandPool, size_t count);

    ~CommandBuffers();

    CommandBuffers(const CommandBuffers&) = delete;

    CommandBuffers(CommandBuffers&& _f) = delete;

    CommandBuffers& operator=(const CommandBuffers&) = delete;

    CommandBuffers& operator=(CommandBuffers&&) = delete;

  private:
    std::vector<VkCommandBuffer> m_commandBuffers;
};
