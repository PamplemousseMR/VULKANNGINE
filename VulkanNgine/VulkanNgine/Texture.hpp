#pragma once

#include <filesystem>

class Texture
{
  public:
    Texture(const std::filesystem::path& _path);

    ~Texture();

    Texture(const Texture&) = delete;

    Texture(Texture&&) = delete;

    Texture& operator=(const Texture&) = delete;

    Texture& operator=(Texture&&) = delete;

  private:

    unsigned char* m_data{nullptr};

    int m_width{0};

    int m_height{0};

    int m_chanels{0};
};
