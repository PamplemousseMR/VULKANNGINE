#pragma once

#include "PhysicalDevice.hpp"

class LogicalDevice
{
  public:
    LogicalDevice(const PhysicalDevice& _physicalDevice);

    ~LogicalDevice();

    inline VkDevice get() const { return m_device; }

    inline uint32_t getgraphicQueueIdx() const { return m_graphicQueueIdx; }

    inline uint32_t getcomputeQueueIdx() const { return m_computeQueueIdx; }

    inline uint32_t gettransferQueueIdx() const { return m_transferQueueIdx; }

    inline uint32_t getsparseQueueIdx() const { return m_sparseQueueIdx; }

    inline uint32_t getprotectQueueIdx() const { return m_protectQueueIdx; }

    inline uint32_t getpresentQueueIdx() const { return m_presentQueueIdx; }

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
