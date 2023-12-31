#pragma once

#include "CommandPool.hpp"
#include "LogicalDevice.hpp"

class CommandBuffers
{
  public:
    CommandBuffers(const LogicalDevice& _logicalDevice, const CommandPool& _commandPool, size_t _count);

    ~CommandBuffers();

    CommandBuffers(const CommandBuffers&) = delete;

    CommandBuffers(CommandBuffers&& _f) = delete;

    CommandBuffers& operator=(const CommandBuffers&) = delete;

    CommandBuffers& operator=(CommandBuffers&&) = delete;

    inline const std::vector<VkCommandBuffer>& get() const { return m_commandBuffers; }

    void beginSingleTimeCommand(size_t _index) const;

    void endSingleTimeCommand(size_t _index) const;

  private:
    const LogicalDevice& m_logicalDevice;

    const CommandPool& m_commandPool;

    std::vector<VkCommandBuffer> m_commandBuffers;
};
