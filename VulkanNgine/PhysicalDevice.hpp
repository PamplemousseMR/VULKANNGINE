#pragma once

#include "Instance.hpp"
#include "Surface.hpp"

#include <vector>

class PhysicalDevice
{
  public:
    static std::vector<PhysicalDevice> getDevices(const Instance& _instance, const Surface& _surface);

    struct QueueFamily
    {
        const uint32_t m_index;
        unsigned m_count{0};
        bool m_graphics{false};
        bool m_compute{false};
        bool m_transfer{false};
        bool m_sparseBinding{false};
        bool m_protected{false};
        bool m_present{false};
    };

    inline VkPhysicalDevice get() const { return m_physicalDevice; }

    inline const std::vector<QueueFamily>& getQueueFamilies() const { return m_queueFamilies; }

    inline const std::string& getName() { return m_name; }

    inline bool hasSwapChainSupport() const { return m_swapChain; }

  private:
    static bool checkExtensionSupport(VkPhysicalDevice _device, const std::vector<const char*>& _extensions);

    PhysicalDevice(VkPhysicalDevice _physicalDevice,
                   const std::string& _name,
                   const std::vector<QueueFamily>& _queueFamilies,
                   bool _swapChain);

    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};

    const std::string m_name;

    std::vector<QueueFamily> m_queueFamilies{};

    const bool m_swapChain{false};
};
