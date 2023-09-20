#pragma once

#include "LogicalDevice.hpp"

#include <string>

class ShaderModule
{
  public:
    ShaderModule(const LogicalDevice& _logicalDevice, const std::string& filename);

    ~ShaderModule();

  private:
    const LogicalDevice& m_logicalDevice;

    VkShaderModule m_shaderModule{VK_NULL_HANDLE};
};
