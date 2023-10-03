#include "RenderPass.hpp"

RenderPass::RenderPass(const LogicalDevice& _logicalDevice, VkFormat _format)
  : m_logicalDevice(_logicalDevice)
{
    VkAttachmentDescription attachementDescription{};
    attachementDescription.format = _format;
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

    VkSubpassDependency subpassDependency{};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = 1;
    renderPassCreateInfo.pAttachments = &attachementDescription;
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &subpassDependency;

    if(vkCreateRenderPass(_logicalDevice.get(), &renderPassCreateInfo, nullptr, &m_renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create the render pass");
    }
}

RenderPass::RenderPass(const LogicalDevice& _logicalDevice, VkFormat _colorFormat, VkFormat _depthFormat)
    : m_logicalDevice(_logicalDevice)
{
    VkAttachmentDescription colorAttachementDescription{};
    colorAttachementDescription.format = _colorFormat;
    colorAttachementDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachementDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachementDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachementDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachementDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachementDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachementDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentDescription depthAttachementDescription{};
    depthAttachementDescription.format = _depthFormat;
    depthAttachementDescription.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachementDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachementDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachementDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachementDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachementDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachementDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentReference{};
    colorAttachmentReference.attachment = 0;
    colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentReference{};
    depthAttachmentReference.attachment = 1;
    depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription{};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = 1;
    subpassDescription.pColorAttachments = &colorAttachmentReference;
    subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;

    VkSubpassDependency subpassDependency{};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpassDependency.dstSubpass = 0;
    subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;
    subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachementDescription, depthAttachementDescription };

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassCreateInfo.pAttachments = attachments.data();
    renderPassCreateInfo.subpassCount = 1;
    renderPassCreateInfo.pSubpasses = &subpassDescription;
    renderPassCreateInfo.dependencyCount = 1;
    renderPassCreateInfo.pDependencies = &subpassDependency;

    if (vkCreateRenderPass(_logicalDevice.get(), &renderPassCreateInfo, nullptr, &m_renderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create the render pass");
    }
}

RenderPass::~RenderPass() { vkDestroyRenderPass(m_logicalDevice.get(), m_renderPass, nullptr); }
