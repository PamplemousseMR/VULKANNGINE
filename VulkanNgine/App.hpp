#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

class App
{
  public:
    void run();

  private:
    static constexpr int s_WIDTH = 800;

    static constexpr int s_HEIGHT = 600;

    static const std::vector<const char*> s_VALIDATION_LAYERS;

#ifdef NDEBUG
    static constexpr bool s_ENABLE_VALIDATION_LAYERS = false;
#else
    static constexpr bool s_ENABLE_VALIDATION_LAYERS = true;
#endif

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugUtilCallback(VkDebugUtilsMessageSeverityFlagBitsEXT _severity,
                                                            VkDebugUtilsMessageTypeFlagsEXT _type,
                                                            const VkDebugUtilsMessengerCallbackDataEXT* _callbackData,
                                                            void* _userData);

    void initWindow();

    void initVulkan();

    void createInstance();

    bool checkExtensionSupport(const std::vector<const char*>& _extensions) const;

    bool checkValidationLayerSupport(const std::vector<const char*>& _validationLayers) const;

    void selectPhysicalDevice();

    void createLogicalDevice();

    void mainLoop();

    void cleanup();

    GLFWwindow* m_window{nullptr};

    VkInstance m_instance{VK_NULL_HANDLE};

    VkDebugUtilsMessengerEXT m_debugutilMessenger{VK_NULL_HANDLE};

    VkPhysicalDevice m_physicalDevice{VK_NULL_HANDLE};

    uint32_t m_graphicsFamilyIndex;

    VkDevice m_logicalDevice{VK_NULL_HANDLE};

    VkQueue m_graphicsQueue{VK_NULL_HANDLE};
};
