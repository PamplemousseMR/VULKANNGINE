#include "App.hpp"

#include <stdexcept>

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
        throw std::runtime_error("Unable to initialize glfw");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(s_WIDTH, s_HEIGHT, "VulkanNgine", nullptr, nullptr);

    if(m_window == nullptr)
    {
        throw std::runtime_error("Unable to create glfw window");
    }
}

void App::initVulkan() {}

void App::mainLoop()
{
    while(!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
}

void App::cleanup()
{
    glfwDestroyWindow(m_window);

    glfwTerminate();
}
