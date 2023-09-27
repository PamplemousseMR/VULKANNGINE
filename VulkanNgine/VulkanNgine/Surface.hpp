#pragma once

#include "Instance.hpp"
#include "Window.hpp"

class Surface
{
  public:
    Surface(Window& _window, const Instance& _instance);

    ~Surface();

    Surface(const Surface&) = delete;

    Surface(Surface&& _f) = delete;

    Surface& operator=(const Surface&) = delete;

    Surface& operator=(Surface&&) = delete;

    VkSurfaceKHR get() const { return m_surface; }

  private:
    const Instance& m_instance;

    VkSurfaceKHR m_surface{VK_NULL_HANDLE};
};
