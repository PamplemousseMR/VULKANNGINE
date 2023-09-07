#include "Window.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>

Window::Window(const std::string& _name)
  : m_name(_name)
{
    if(glfwInit() != GLFW_TRUE)
    {
        throw std::runtime_error("failed to initialize glfw");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_window = glfwCreateWindow(s_WIDTH, s_HEIGHT, m_name.c_str(), nullptr, nullptr);

    if(m_window == nullptr)
    {
        throw std::runtime_error("failed to create window");
    }
}

Window::~Window()
{
    glfwDestroyWindow(m_window);

    glfwTerminate();
}

void Window::run()
{
    while(!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
    }
}

std::vector<const char*> Window::getRequiredInstanceExtensions() const
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    return std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
}

const std::string& Window::getName() const { return m_name; }
