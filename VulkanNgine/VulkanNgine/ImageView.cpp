#include "ImageView.hpp"

ImageView::ImageView(const LogicalDevice& _logicalDevice, const Image& _image)
  : m_logicalDevice(_logicalDevice)

{
	VkImageViewCreateInfo imageViewCreateInfo{};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.image = _image.get();
	imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount = 1;

	if(vkCreateImageView(_logicalDevice.get(), &imageViewCreateInfo, nullptr, &m_imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create image view");
	}
}

ImageView::~ImageView()
{
	vkDestroyImageView(m_logicalDevice.get(), m_imageView, nullptr);
}
