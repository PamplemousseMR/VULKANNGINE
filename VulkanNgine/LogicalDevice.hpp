#pragma once

#include "PhysicalDevice.hpp"

class LogicalDevice
{
  public:
    LogicalDevice(const PhysicalDevice& _physicalDevice,
                  VkQueueFlags _queueFlags,
                  bool _surfaceSupport,
                  bool _swapChainSupport);

    ~LogicalDevice();

  private:
    static constexpr float s_QUEUE_PRIORITY{1.0f};

    VkDevice m_device{VK_NULL_HANDLE};

    VkQueue m_graphicQueue{VK_NULL_HANDLE};

    VkQueue m_computeQueue{VK_NULL_HANDLE};

    VkQueue m_transferQueue{VK_NULL_HANDLE};

    VkQueue m_sparseQueue{VK_NULL_HANDLE};

    VkQueue m_protectQueue{VK_NULL_HANDLE};

    VkQueue m_presentQueue{VK_NULL_HANDLE};
};
