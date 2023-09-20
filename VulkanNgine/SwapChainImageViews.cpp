#include "SwapChainImageViews.hpp"

SwapChainImageViews::SwapChainImageViews(const SwapChain& _swapChain, const LogicalDevice& _logicalDevice)
  : m_logicalDevice(_logicalDevice)

{
    const std::vector<VkImage>& swapChainImages = _swapChain.getImages();
    m_imageViews.resize(swapChainImages.size());

    for(size_t i = 0; i < swapChainImages.size(); ++i)
    {
        VkImageViewCreateInfo imageViewCreateInfo{};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = swapChainImages[i];

        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.format = _swapChain.getFormat().format;

        imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        if(vkCreateImageView(_logicalDevice.get(), &imageViewCreateInfo, nullptr, &m_imageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create swap chain image view");
        }
    }
}

SwapChainImageViews::~SwapChainImageViews()
{
    for(VkImageView imageView : m_imageViews)
    {
        vkDestroyImageView(m_logicalDevice.get(), imageView, nullptr);
    }
}
