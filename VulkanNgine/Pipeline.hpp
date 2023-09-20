#pragma once

#include "LogicalDevice.hpp"
#include "RenderPass.hpp"
#include "ShaderModule.hpp"
#include "SwapChain.hpp"

class Pipeline
{
  public:
    Pipeline(const LogicalDevice& _logicalDevice,
             const ShaderModule& _vertexShaderModule,
             const ShaderModule& _fragmentShaderModule,
             const RenderPass& _renderPass,
             const SwapChain& _swapChain);

    ~Pipeline();

  private:
    const LogicalDevice& m_logicalDevice;

    VkPipelineLayout m_layout{VK_NULL_HANDLE};

    VkPipeline m_pipeline{VK_NULL_HANDLE};
};
