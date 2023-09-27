#pragma once

#include "LogicalDevice.hpp"

#include <glm/glm.hpp>

class Buffer
{
  public:
    struct Vertex
    {
        glm::vec2 m_pos;
        glm::vec3 m_color;
    };

    Buffer(const LogicalDevice& _logicalDevice, VkBufferUsageFlags _usage, const std::vector<Buffer::Vertex>& _data);

    ~Buffer();

    Buffer(const Buffer&) = delete;

    Buffer(Buffer&& _f) = delete;

    Buffer& operator=(const Buffer&) = delete;

    Buffer& operator=(Buffer&&) = delete;

    inline VkBuffer get() const { return m_buffer; }

    inline VkMemoryRequirements getRequirements() const { return m_requirements; }

    inline size_t getSize() const { return sizeof(m_data[0]) * m_data.size(); }

    inline const std::vector<Buffer::Vertex>& getData() const { return m_data; }

  private:
    const LogicalDevice& m_logicalDevice;

    std::vector<Buffer::Vertex> m_data;

    VkBuffer m_buffer;

    VkMemoryRequirements m_requirements;
};
