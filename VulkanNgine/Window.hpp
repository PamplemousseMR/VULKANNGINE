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

    void create();

    void run();

    void destroy();

    std::vector<const char*> getRequiredInstanceExtensions() const;

    const std::string& getName() const;

  private:
    const std::string m_name;

    GLFWwindow* m_window{nullptr};
};
