#include "Texture.hpp"

#include <stb/stb_image.h>

Texture::Texture(const std::filesystem::path& _path)
{
    m_data = stbi_load(_path.string().c_str(), &m_width, &m_height, &m_chanels, STBI_rgb_alpha);
    m_chanels = 4; // forced with STBI_rgb_alpha

    if (!m_data) {
        throw std::runtime_error("Failed to load the texture: " + _path.string());
    }

    m_mipLevels = static_cast<int>(std::floor(std::log2(std::max(m_width, m_height)))) + 1;
}

Texture::~Texture()
{
    stbi_image_free(m_data);
}