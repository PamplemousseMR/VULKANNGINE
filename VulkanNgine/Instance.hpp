#pragma once

#include "Window.hpp"

#include <vulkan/vulkan.hpp>

#include <vector>

class Instance
{
  public:
    Instance(const Window& _window);

    void create();

    void destroy();

    operator VkInstance() const;

  private:
    static const std::vector<const char*> s_VALIDATION_LAYERS;

#ifdef NDEBUG
    static constexpr bool s_ENABLE_VALIDATION_LAYERS = false;
#else
    static constexpr bool s_ENABLE_VALIDATION_LAYERS = true;
#endif

    bool checkExtensionSupport(const std::vector<const char*>& _extensions) const;

    bool checkLayerSupport(const std::vector<const char*>& _layers) const;

    bool m_debugUtilSupported{false};

    const Window& m_window;

    VkInstance m_instance{VK_NULL_HANDLE};

    VkDebugUtilsMessengerEXT m_debugUtilMessengerExt{VK_NULL_HANDLE};
};
