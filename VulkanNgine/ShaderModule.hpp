#pragma once

#include "LogicalDevice.hpp"

#include <string>

class ShaderModule
{
  public:
    ShaderModule(const LogicalDevice& _logicalDevice, const std::string& filename);

    ~ShaderModule();

    inline VkShaderModule get() const { return m_shaderModule; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkShaderModule m_shaderModule{VK_NULL_HANDLE};
};
