#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class App
{
  public:
    void run();

  private:
    static constexpr int s_WIDTH = 800;

    static constexpr int s_HEIGHT = 600;

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

    GLFWwindow* m_window{nullptr};
};
