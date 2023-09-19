#include "Instance.hpp"

#include "logger.hpp"

#include <iostream>

namespace {

bool checkExtensionSupport(const std::vector<const char*>& _extensions)
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
            VKNGINE_LOG_WARNING("Extension '" << extensionName << "' not available");
            return false;
        }
    }

    return true;
}

bool checkLayerSupport(const std::vector<const char*>& _layers)
{
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
            VKNGINE_LOG_WARNING("Layer '" << layerName << "' not available");
            return false;
        }
    }

    return true;
}

}

const std::vector<const char*> Instance::s_VALIDATION_LAYERS = {"VK_LAYER_LUNARG_standard_validation"};

std::string typeToString(VkDebugUtilsMessageTypeFlagsEXT _type)
{
    switch(_type)
    {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: return "general";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: return "validation";
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: return "performance";
        default: break;
    }

    if(_type & (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT))
    {
        return "general|validation|performance";
    }
    else if(_type & (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT))
    {
        return "general|validation";
    }
    else if(_type & (VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT))
    {
        return "validation|performance";
    }
    else if(_type & (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT))
    {
        return "general|performance";
    }

    return "unknonw";
}

VkBool32 debugUtilCallback(VkDebugUtilsMessageSeverityFlagBitsEXT _severity,
                           VkDebugUtilsMessageTypeFlagsEXT _type,
                           const VkDebugUtilsMessengerCallbackDataEXT* _callbackData,
                           void*)
{
    if(_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        VKNGINE_LOG_ERROR("(" << typeToString(_type) << ") " << _callbackData->pMessage);
    }
    else if(_type & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        VKNGINE_LOG_WARNING("(" << typeToString(_type) << ") " << _callbackData->pMessage);
    }
    else if(_type & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    {
        VKNGINE_LOG_INFO("(" << typeToString(_type) << ") " << _callbackData->pMessage);
    }
    else if(_type & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        VKNGINE_LOG_VERBOSE("(" << typeToString(_type) << ") " << _callbackData->pMessage);
    }
    else
    {
        VKNGINE_LOG_UNKNOWN("(" << typeToString(_type) << ") " << _callbackData->pMessage);
    }

    return VK_FALSE;
}

Instance::Instance(const Window& _window)
{
    // Get required extensions
    std::vector<const char*> requiredExtensions = _window.getRequiredInstanceExtensions();

    for(const std::string& ex : requiredExtensions)
    {
        VKNGINE_LOG_INFO("Required extension " << ex);
    }

    // Check required extensions
    if(!checkExtensionSupport(requiredExtensions))
    {
        throw std::runtime_error("One or more required extensions is not available");
    }

    // Optional extensions
    m_debugUtilSupported = checkExtensionSupport({VK_EXT_DEBUG_UTILS_EXTENSION_NAME});
    if(m_debugUtilSupported)
    {
        VKNGINE_LOG_INFO("Optional extension " << VK_EXT_DEBUG_UTILS_EXTENSION_NAME << ": true");
        requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    else
    {
        VKNGINE_LOG_INFO("Optional extension " << VK_EXT_DEBUG_UTILS_EXTENSION_NAME << ": false");
    }

    // Check layers
    if(s_ENABLE_VALIDATION_LAYERS)
    {
        for(const std::string& la : s_VALIDATION_LAYERS)
        {
            VKNGINE_LOG_INFO("Required layer " << la);
        }

        if(!checkLayerSupport(s_VALIDATION_LAYERS))
        {
            throw std::runtime_error("One or more required layers is not available");
        }
    }

    // Application information
    VkApplicationInfo applicationInfo{};
    applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pApplicationName = _window.getName().c_str();
    applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 1, 0);
    applicationInfo.pEngineName = "VkEngine";
    applicationInfo.engineVersion = VK_MAKE_VERSION(1, 1, 0);
    applicationInfo.apiVersion = VK_API_VERSION_1_1;

    // Instance information
    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &applicationInfo;
    instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
    instanceCreateInfo.enabledLayerCount =
      s_ENABLE_VALIDATION_LAYERS ? static_cast<uint32_t>(s_VALIDATION_LAYERS.size()) : 0;
    instanceCreateInfo.ppEnabledLayerNames = s_ENABLE_VALIDATION_LAYERS ? s_VALIDATION_LAYERS.data() : nullptr;

    // Setup debug utils
    VkDebugUtilsMessengerCreateInfoEXT debugUtilMessengerCreateInfo{};
    if(m_debugUtilSupported)
    {
        debugUtilMessengerCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugUtilMessengerCreateInfo.messageSeverity =
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugUtilMessengerCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                                   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                                   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugUtilMessengerCreateInfo.pfnUserCallback = debugUtilCallback;
        debugUtilMessengerCreateInfo.pUserData = nullptr;

        instanceCreateInfo.pNext = &debugUtilMessengerCreateInfo;
    }

    // Create the instance
    if(vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create instance");
    }

    if(m_debugUtilSupported)
    {
        auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
        if(func != nullptr)
        {
            func(m_instance, &debugUtilMessengerCreateInfo, nullptr, &m_debugUtilMessengerExt);
        }
        else
        {
            throw std::runtime_error("Failed to get instance process address");
        }
    }
}

Instance::~Instance()
{
    if(m_debugUtilSupported)
    {
        auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
        if(func != nullptr)
        {
            func(m_instance, m_debugUtilMessengerExt, nullptr);
        }
    }

    vkDestroyInstance(m_instance, nullptr);
}
