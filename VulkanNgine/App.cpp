#include "App.hpp"

#include <iostream>
#include <stdexcept>

const std::vector<const char*> App::s_VALIDATION_LAYERS = {"VK_LAYER_LUNARG_standard_validation"};

std::string toString(VkDebugUtilsMessageTypeFlagsEXT type)
{
    switch(type)
    {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: return "general";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: return "validation";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: return "performance";
        default: return "unknow VkDebugUtilsMessageTypeFlagsEXT";
    }
}

void App::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

VkBool32 App::debugUtilCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                VkDebugUtilsMessageTypeFlagsEXT type,
                                const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
                                void*)
{
    switch(type)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            std::cout << "Verbose (" << toString(type) << "): " << callbackData->pMessage << std::endl;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            std::cout << "Info (" << toString(type) << "): " << callbackData->pMessage << std::endl;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            std::cerr << "Warning (" << toString(type) << "): " << callbackData->pMessage << std::endl;
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            std::cerr << "Error (" << toString(type) << "): " << callbackData->pMessage << std::endl;
            break;
        default: std::cout << "Unknow (" << toString(type) << "): " << callbackData->pMessage << std::endl; break;
    }

    return VK_FALSE;
}

void App::initWindow()
{
    if(glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("failed to initialize glfw");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(s_WIDTH, s_HEIGHT, "VulkanNgine", nullptr, nullptr);

    if(m_window == nullptr)
    {
        throw std::runtime_error("failed to create glfw window");
    }
}

void App::initVulkan() { createInstance(); }

void App::createInstance()
{
    // Application information
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanNgine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Needed extensions
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    for(const std::string& ex : requiredExtensions)
    {
        std::cout << "Required extension " << ex << std::endl;
    }

    // Check required extensions
    if(!checkExtensionSupport(requiredExtensions))
    {
        throw std::runtime_error("One or more required extensions is not available");
    }

    const bool isDebugUtilSupported = checkExtensionSupport({VK_EXT_DEBUG_UTILS_EXTENSION_NAME});
    if(isDebugUtilSupported)
    {
        std::cout << "Optional extension " << VK_EXT_DEBUG_UTILS_EXTENSION_NAME << " available" << std::endl;
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    else
    {
        std::cout << "Optional extension " << VK_EXT_DEBUG_UTILS_EXTENSION_NAME << " not available" << std::endl;
    }

    // Check layers
    if(s_ENABLE_VALIDATION_LAYERS)
    {
        for(const std::string& la : s_VALIDATION_LAYERS)
        {
            std::cout << "Required layer " << la << std::endl;
        }

        if(!checkValidationLayerSupport(s_VALIDATION_LAYERS))
        {
            throw std::runtime_error("One or more required layers is not available");
        }
    }

    // Instance information
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    createInfo.enabledLayerCount = s_ENABLE_VALIDATION_LAYERS ? static_cast<uint32_t>(s_VALIDATION_LAYERS.size()) : 0;
    createInfo.ppEnabledLayerNames = s_ENABLE_VALIDATION_LAYERS ? s_VALIDATION_LAYERS.data() : nullptr;

    // Create the instance
    if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance");
    }

    // Setup debug utils
    if(isDebugUtilSupported)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfoExt{};
        createInfoExt.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfoExt.messageSeverity =
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfoExt.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfoExt.pfnUserCallback = debugUtilCallback;
        createInfoExt.pUserData = nullptr;

        auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
        if(func != nullptr)
        {
            func(m_instance, &createInfoExt, nullptr, &m_debugutilMessengerExt);
        }
        else
        {
            throw std::runtime_error("failed to get instance process address");
        }
    }
}

bool App::checkExtensionSupport(const std::vector<const char*>& _extensions) const
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
    for(const char* extensionName : _extensions)
    {
        bool extensionFound = false;

        for(const auto& ex : availableExtensions)
        {
            if(strcmp(extensionName, ex.extensionName) == 0)
            {
                extensionFound = true;
                break;
            }
        }

        if(!extensionFound)
        {
            std::cerr << "Extension: " << extensionName << " not available" << std::endl;
            return false;
        }
    }

    return true;
}

bool App::checkValidationLayerSupport(const std::vector<const char*>& _layers) const
{
    // Available layer
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    for(const char* layerName : _layers)
    {
        bool layerFound = false;

        for(const auto& la : availableLayers)
        {
            if(strcmp(layerName, la.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if(!layerFound)
        {
            std::cerr << "Layer: " << layerName << " not available" << std::endl;
            return false;
        }
    }

    return true;
}

void App::mainLoop()
{
    while(!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
}

void App::cleanup()
{
    const bool isDebugUtilSupported = checkExtensionSupport({VK_EXT_DEBUG_UTILS_EXTENSION_NAME});
    if(isDebugUtilSupported)
    {
        auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
        if(func != nullptr)
        {
            func(m_instance, m_debugutilMessengerExt, nullptr);
        }
    }

    vkDestroyInstance(m_instance, nullptr);

    glfwDestroyWindow(m_window);

    glfwTerminate();
}
