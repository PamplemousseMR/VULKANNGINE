#pragma once

#include "Window.hpp"

#include <vulkan/vulkan.hpp>

#include <vector>

class Instance
{
  public:
    static const std::vector<const char*> s_VALIDATION_LAYERS;

#ifdef NDEBUG
    static constexpr bool s_ENABLE_VALIDATION_LAYERS = false;
#else
    static constexpr bool s_ENABLE_VALIDATION_LAYERS = true;
#endif

    Instance(const Window& _window);

    ~Instance();

    inline VkInstance get() const { return m_instance; }

  private:
    bool m_debugUtilSupported{false};

    VkInstance m_instance{VK_NULL_HANDLE};

    VkDebugUtilsMessengerEXT m_debugUtilMessengerExt{VK_NULL_HANDLE};
};
