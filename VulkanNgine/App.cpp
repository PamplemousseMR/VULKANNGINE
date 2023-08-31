#include "App.hpp"

#include <iostream>
#include <stdexcept>

const std::vector<std::string> App::s_VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

void App::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
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
    std::vector<std::string> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    for(const std::string& ex : requiredExtensions)
    {
        std::cout << "Required extension " << ex << std::endl;
    }

    // Check required extensions
    if(!checkExtensionSupport(requiredExtensions))
    {
        throw std::runtime_error("One or more required extensions is not available");
    }

    // Check layers
    if(s_ENABLE_VALIDATION_LAYERS)
    {
        for(const std::string& la : s_VALIDATION_LAYERS)
        {
            std::cout << "Required layer " << la << std::endl;
        }
    }

    if(s_ENABLE_VALIDATION_LAYERS && !checkValidationLayerSupport(s_VALIDATION_LAYERS))
    {
        throw std::runtime_error("One or more required layers is not available");
    }

    // Instance information
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    // Create the instance
    if(vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}

bool App::checkExtensionSupport(const std::vector<std::string>& _extensions) const
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
    for(const VkExtensionProperties& ex : availableExtensions)
    {
        std::cout << "Available extension: " << ex.extensionName << " " << ex.specVersion << std::endl;
    }

    for(const std::string& extensionName : _extensions)
    {
        bool extensionFound = false;

        for(const auto& ex : availableExtensions)
        {
            if(strcmp(extensionName.c_str(), ex.extensionName) == 0)
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

bool App::checkValidationLayerSupport(const std::vector<std::string>& _layers) const
{
    // Available layer
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    for(const VkLayerProperties& la : availableLayers)
    {
        std::cout << "Available layer: " << la.layerName << " " << la.specVersion << std::endl;
    }

    for(const std::string& layerName : _layers)
    {
        bool layerFound = false;

        for(const auto& la : availableLayers)
        {
            if(strcmp(layerName.c_str(), la.layerName) == 0)
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
    vkDestroyInstance(m_instance, nullptr);

    glfwDestroyWindow(m_window);

    glfwTerminate();
}
