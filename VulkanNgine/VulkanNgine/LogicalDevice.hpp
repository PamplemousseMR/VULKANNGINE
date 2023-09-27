#pragma once

#include "PhysicalDevice.hpp"

class LogicalDevice
{
  public:
    LogicalDevice(const PhysicalDevice& _physicalDevice);

    ~LogicalDevice();

    LogicalDevice(const LogicalDevice&) = delete;

    LogicalDevice(LogicalDevice&& _f) = delete;

    LogicalDevice& operator=(const LogicalDevice&) = delete;

    LogicalDevice& operator=(LogicalDevice&&) = delete;

    inline VkDevice get() const { return m_device; }

    inline VkQueue getGraphicQueue() const { return m_graphicQueue; }

    inline uint32_t getGraphicQueueIdx() const { return m_graphicQueueIdx; }

    inline uint32_t getComputeQueueIdx() const { return m_computeQueueIdx; }

    inline VkQueue getTransferQueue() const { return m_transferQueue; }

    inline uint32_t getTransferQueueIdx() const { return m_transferQueueIdx; }

    inline uint32_t getSparseQueueIdx() const { return m_sparseQueueIdx; }

    inline uint32_t getProtectQueueIdx() const { return m_protectQueueIdx; }

    inline VkQueue getPresentQueue() const { return m_presentQueue; }

    inline uint32_t getPresentQueueIdx() const { return m_presentQueueIdx; }

  private:
    static constexpr float s_QUEUE_PRIORITY{1.0f};

    VkDevice m_device{VK_NULL_HANDLE};

    VkQueue m_graphicQueue{VK_NULL_HANDLE};

    uint32_t m_graphicQueueIdx{0};

    VkQueue m_computeQueue{VK_NULL_HANDLE};

    uint32_t m_computeQueueIdx{0};

    VkQueue m_transferQueue{VK_NULL_HANDLE};

    uint32_t m_transferQueueIdx{0};

    VkQueue m_sparseQueue{VK_NULL_HANDLE};

    uint32_t m_sparseQueueIdx{0};

    VkQueue m_protectQueue{VK_NULL_HANDLE};

    uint32_t m_protectQueueIdx{0};

    VkQueue m_presentQueue{VK_NULL_HANDLE};

    uint32_t m_presentQueueIdx{0};
};
