#include "App.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>

const std::vector<const char*> App::s_VALIDATION_LAYERS = {"VK_LAYER_LUNARG_standard_validation"};

std::string toString(VkDebugUtilsMessageTypeFlagsEXT _type)
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

void App::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

VkBool32 App::debugUtilCallback(VkDebugUtilsMessageSeverityFlagBitsEXT _severity,
                                VkDebugUtilsMessageTypeFlagsEXT _type,
                                const VkDebugUtilsMessengerCallbackDataEXT* _callbackData,
                                void*)
{
    if(_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        std::cerr << "Error (" << toString(_type) << "): " << _callbackData->pMessage << std::endl;
    }
    else if(_type & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        std::cerr << "Warning (" << toString(_type) << "): " << _callbackData->pMessage << std::endl;
    }
    else if(_type & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    {
        std::cout << "Info (" << toString(_type) << "): " << _callbackData->pMessage << std::endl;
    }
    else if(_type & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        std::cout << "Verbose (" << toString(_type) << "): " << _callbackData->pMessage << std::endl;
    }
    else
    {
        std::cout << "Unknown (" << toString(_type) << "): " << _callbackData->pMessage << std::endl;
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

void App::initVulkan()
{
    createInstance();
    selectPhysicalDevice();
    createLogicalDevice();
}

void App::createInstance()
{
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

    // Optional extensions
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

    // Application information
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "VulkanNgine";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Instance information
    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();
    instanceInfo.enabledLayerCount = s_ENABLE_VALIDATION_LAYERS ? static_cast<uint32_t>(s_VALIDATION_LAYERS.size()) : 0;
    instanceInfo.ppEnabledLayerNames = s_ENABLE_VALIDATION_LAYERS ? s_VALIDATION_LAYERS.data() : nullptr;

    // Setup debug utils
    VkDebugUtilsMessengerCreateInfoEXT debugutilMessengerInfo{};
    if(isDebugUtilSupported)
    {
        debugutilMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugutilMessengerInfo.messageSeverity =
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugutilMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugutilMessengerInfo.pfnUserCallback = debugUtilCallback;
        debugutilMessengerInfo.pUserData = nullptr;

        instanceInfo.pNext = &debugutilMessengerInfo;
    }

    // Create the instance
    if(vkCreateInstance(&instanceInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance");
    }

    auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
    if(func != nullptr)
    {
        func(m_instance, &debugutilMessengerInfo, nullptr, &m_debugutilMessenger);
    }
    else
    {
        throw std::runtime_error("failed to get instance process address");
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

void App::selectPhysicalDevice()
{
    // Device
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);

    if(deviceCount == 0)
    {
        throw std::runtime_error("No device found");
    }

    std::vector<VkPhysicalDevice> availableDevices(deviceCount);
    vkEnumeratePhysicalDevices(m_instance, &deviceCount, availableDevices.data());

    for(const VkPhysicalDevice& device : availableDevices)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        std::cout << "Device " << deviceProperties.deviceID << " name " << deviceProperties.deviceName << std::endl;

        // Queue families
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> availableQueueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, availableQueueFamilies.data());

        for(size_t i = 0; i < availableQueueFamilies.size(); ++i)
        {
            if(availableQueueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_graphicsFamilyIndex = static_cast<uint32_t>(i);

                m_physicalDevice = device;
                std::cout << "Selected physical device " << deviceProperties.deviceID << " name "
                          << deviceProperties.deviceName << std::endl;
                break;
            }
        }
    }

    if(m_physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("No available physical device");
    }
}

void App::createLogicalDevice()
{
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = m_graphicsFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;
    createInfo.enabledLayerCount = 0;

    if(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
    {
        throw std::runtime_error("Error while creating the logical device");
    }

    vkGetDeviceQueue(m_logicalDevice, m_graphicsFamilyIndex, 0, &m_graphicsQueue);
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
    vkDestroyDevice(m_logicalDevice, nullptr);

    const bool isDebugUtilSupported = checkExtensionSupport({VK_EXT_DEBUG_UTILS_EXTENSION_NAME});
    if(isDebugUtilSupported)
    {
        auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
          vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
        if(func != nullptr)
        {
            func(m_instance, m_debugutilMessenger, nullptr);
        }
    }

    vkDestroyInstance(m_instance, nullptr);

    glfwDestroyWindow(m_window);

    glfwTerminate();
}
