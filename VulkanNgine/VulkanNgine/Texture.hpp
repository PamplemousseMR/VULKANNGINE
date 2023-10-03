#pragma once

#include <filesystem>

#include <vulkan/vulkan.hpp>

class Texture
{
  public:
    Texture(const std::filesystem::path& _path);

    ~Texture();

    Texture(const Texture&) = delete;

    Texture(Texture&&) = delete;

    Texture& operator=(const Texture&) = delete;

    Texture& operator=(Texture&&) = delete;

    inline VkDeviceSize getSize() const { return static_cast<VkDeviceSize>(m_width * m_height * m_chanels); }

    inline const unsigned char* data() const { return m_data; }

    inline int width() const { return m_width; }

    inline int height() const { return m_height; }

    inline int level() const { return m_mipLevels; }

  private:

    unsigned char* m_data{nullptr};

    int m_width{0};

    int m_height{0};

    int m_chanels{0};

    int m_mipLevels{0};
};
