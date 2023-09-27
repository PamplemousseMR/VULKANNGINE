#pragma once

#include "LogicalDevice.hpp"

#include <string>

class ShaderModule
{
  public:
    ShaderModule(const LogicalDevice& _logicalDevice, const std::string& filename);

    ~ShaderModule();

    ShaderModule(const ShaderModule&) = delete;

    ShaderModule(ShaderModule&& _f) = delete;

    ShaderModule& operator=(const ShaderModule&) = delete;

    ShaderModule& operator=(ShaderModule&&) = delete;

    inline VkShaderModule get() const { return m_shaderModule; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkShaderModule m_shaderModule{VK_NULL_HANDLE};
};
