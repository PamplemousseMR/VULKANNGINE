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

    void initWindow();

    void initVulkan();

    void createInstance();

    void mainLoop();

    void cleanup();

    GLFWwindow* m_window{nullptr};

    VkInstance m_instance{nullptr};
};
