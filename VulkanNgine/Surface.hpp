#pragma once

#include "Instance.hpp"
#include "Window.hpp"

class Surface
{
  public:
    Surface(Window& _window, const Instance& _instance);

    ~Surface();

    VkSurfaceKHR get() const { return m_surface; }

  private:
    const Instance& m_instance;

    VkSurfaceKHR m_surface{VK_NULL_HANDLE};
};
