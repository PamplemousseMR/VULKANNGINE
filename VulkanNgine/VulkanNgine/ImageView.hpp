#pragma once

#include "LogicalDevice.hpp"
#include "Image.hpp"

class ImageView
{
  public:
    ImageView(const LogicalDevice& _logicalDevice, const Image& _image, VkFormat _format, VkImageAspectFlags _aspectFlags);

    ~ImageView();

    ImageView(const ImageView&) = delete;

    ImageView(ImageView&&) = delete;

    ImageView& operator=(const ImageView&) = delete;

    ImageView& operator=(ImageView&&) = delete;

    inline VkImageView get() const { return m_imageView; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkImageView m_imageView{VK_NULL_HANDLE};
};
