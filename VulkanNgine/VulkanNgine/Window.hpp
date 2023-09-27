#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

class Window
{
  public:
    static constexpr int s_WIDTH = 800;

    static constexpr int s_HEIGHT = 600;

    Window(const std::string& _name);

    ~Window();

    Window(const Window&) = delete;

    Window(Window&& _f) = delete;

    Window& operator=(const Window&) = delete;

    Window& operator=(Window&&) = delete;

    bool shouldClose();

    void poolEvent();

    std::vector<const char*> getRequiredInstanceExtensions() const;

    const std::string& getName() const;

    inline GLFWwindow* get() { return m_window; }

  private:
    const std::string m_name;

    GLFWwindow* m_window{nullptr};
};
