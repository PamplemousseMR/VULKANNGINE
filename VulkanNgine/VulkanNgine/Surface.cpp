#include "Surface.hpp"

Surface::Surface(Window& _window, const Instance& _instance)
  : m_instance(_instance)
{
    if(glfwCreateWindowSurface(_instance.get(), _window.get(), nullptr, &m_surface))
    {
        throw std::runtime_error("Failed to create the surface");
    }
}

Surface::~Surface() { vkDestroySurfaceKHR(m_instance.get(), m_surface, nullptr); }
