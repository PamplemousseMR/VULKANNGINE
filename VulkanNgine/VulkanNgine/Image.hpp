#pragma once

#include "LogicalDevice.hpp"

#include "Texture.hpp"

class Image
{
  public:
    Image(const LogicalDevice& _logicalDevice, const Texture& _texture, VkImageUsageFlags _usage);

    ~Image();

    Image(const Image&) = delete;

    Image(Image&&) = delete;

    Image& operator=(const Image&) = delete;

    Image& operator=(Image&&) = delete;

    inline VkImage get() const { return m_image; }

    inline VkMemoryRequirements getRequirements() const { return m_requirements; }

  private:
    const LogicalDevice& m_logicalDevice;

    VkImage m_image;

    VkMemoryRequirements m_requirements;
};
