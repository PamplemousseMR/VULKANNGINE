#include "RenderPass.hpp"

RenderPass::RenderPass(const LogicalDevice& _logicalDevice, const SwapChain& _swapChain)
  : m_logicalDevice(_logicalDevice)
{
    VkAttachmentDescription attachementDescription{};
    attachementDescription.format = _swapChain.getFormat().format;
    attachementDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    attachementDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachementDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachementDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachementDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachementDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachementDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference attachmentReference{};
    attachmentReference.attachment = 0;
    attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription{};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &attachmentReference;

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &attachementDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;

    if(vkCreateRenderPass(_logicalDevice.get(), &renderPassCreateInfo, nullptr, &m_renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create the render pass");
    }
}

RenderPass::~RenderPass() { vkDestroyRenderPass(m_logicalDevice.get(), m_renderPass, nullptr); }
