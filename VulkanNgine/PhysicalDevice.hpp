#pragma once

#include "Instance.hpp"

#include <vector>

class PhysicalDevice
{
  public:
    static std::vector<PhysicalDevice> getDevices(const Instance& _instance);

    struct QueueFamily
    {
        const size_t m_index;
        unsigned m_count{0};
        bool m_graphics{false};
        bool m_compute{false};
        bool m_transfer{false};
        bool m_sparseBinding{false};
        bool m_protected{false};
    };

    inline const std::vector<QueueFamily>& getQueueFamilies() const { return m_queueFamilies; }

    inline const std::string& getName() { return m_name; }

  private:
    PhysicalDevice(const Instance& _instance, const std::string& _name, const std::vector<QueueFamily>& _queueFamilies);

    const Instance& m_instance;

    const std::string m_name;

    std::vector<QueueFamily> m_queueFamilies{};

    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};
};
