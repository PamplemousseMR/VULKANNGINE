#pragma once

#include "VulkanNgine/LogicalDevice.hpp"
#include "VulkanNgine/RenderPass.hpp"
#include "VulkanNgine/ShaderModule.hpp"
#include "VulkanNgine/SwapChain.hpp"

class Pipeline
{
  public:
    Pipeline(const LogicalDevice& _logicalDevice,
             const ShaderModule& _vertexShaderModule,
             const ShaderModule& _fragmentShaderModule,
             const RenderPass& _renderPass,
             VkExtent2D _size);

    ~Pipeline();

    Pipeline(const Pipeline&) = delete;

    Pipeline(Pipeline&& _f) = delete;

    Pipeline& operator=(const Pipeline&) = delete;

    Pipeline& operator=(Pipeline&&) = delete;

    inline VkPipeline get() const { return m_pipeline; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkPipelineLayout m_layout{VK_NULL_HANDLE};

    VkPipeline m_pipeline{VK_NULL_HANDLE};
};
