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

    inline VkDescriptorSetLayout getDescriptorSetlayout() const { return m_descriptorSetLayout; }

    inline VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkDescriptorSetLayout m_descriptorSetLayout{VK_NULL_HANDLE};

    VkPipelineLayout m_pipelineLayout{VK_NULL_HANDLE};

    VkPipeline m_pipeline{VK_NULL_HANDLE};
};
