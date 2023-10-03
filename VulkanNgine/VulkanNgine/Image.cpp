#include "Image.hpp"

Image::Image(const LogicalDevice& _logicalDevice, uint32_t _width, uint32_t _height, uint32_t _level, VkFormat _format, VkImageTiling _tiling, VkImageUsageFlags _usage)
  : m_logicalDevice(_logicalDevice)
{
    VkImageCreateInfo imageCreateInfo{};
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.extent.width = _width;
    imageCreateInfo.extent.height = _height;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = _level;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.format = _format;
    imageCreateInfo.tiling = _tiling;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateInfo.usage = _usage;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.flags = 0;

    if (vkCreateImage(_logicalDevice.get(), &imageCreateInfo, nullptr, &m_image) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create image");
    }

    vkGetImageMemoryRequirements(_logicalDevice.get(), m_image, &m_requirements);
}

Image::~Image()
{
    vkDestroyImage(m_logicalDevice.get(), m_image, nullptr);
}