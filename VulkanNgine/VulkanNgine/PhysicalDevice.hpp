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

    struct SwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR m_capabilities;
        std::vector<VkSurfaceFormatKHR> m_formats;
        std::vector<VkPresentModeKHR> m_presentModes;
    };

    PhysicalDevice(const PhysicalDevice&) = delete;

    PhysicalDevice(PhysicalDevice&& _d);

    PhysicalDevice& operator=(const PhysicalDevice&) = delete;

    PhysicalDevice& operator=(PhysicalDevice&&) = delete;

    inline VkPhysicalDevice get() const { return m_physicalDevice; }

    inline const std::vector<QueueFamily>& getQueueFamilies() const { return m_queueFamilies; }

    inline const std::string& getName() const { return m_name; }

    inline bool hasSwapChainSupport() const { return m_swapChainSupport; }

    inline bool hasSamplerAnisotropySupport() const { return m_samplerAnisotropySupport; }

    inline const SwapChainSupportDetails getSwapChainSupportDetails() const { return m_swapChainSupportDetails; }

    uint32_t findMemoryType(uint32_t _typeFilter, VkMemoryPropertyFlags _properties) const;

    VkFormat findSupportedFormat(const std::vector<VkFormat>& _candidates, VkImageTiling _tiling, VkFormatFeatureFlags _features) const;

  private:
    PhysicalDevice(VkPhysicalDevice _physicalDevice,
                   const std::string& _name,
                   const std::vector<QueueFamily>& _queueFamilies,
                   bool _swapChainSupport,
                   bool _samplerAnisotropySupport,
                   const SwapChainSupportDetails _swapChainSupportDetails);

    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};

    const std::string m_name;

    const std::vector<QueueFamily> m_queueFamilies;

    const bool m_swapChainSupport;

    const bool m_samplerAnisotropySupport;

    const SwapChainSupportDetails m_swapChainSupportDetails;
};
