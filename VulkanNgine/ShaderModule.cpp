#include "ShaderModule.hpp"

#include "logger.hpp"

#include <fstream>
#include <vector>

namespace {

std::vector<char> readShaderBinaryFile(const std::string& filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if(!file.is_open())
    {
        throw std::runtime_error("Failed to open " + filename);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), static_cast<std::streamsize>(fileSize));
    file.close();

    return buffer;
}

}

ShaderModule::ShaderModule(const LogicalDevice& _logicalDevice, const std::string& _filename)
  : m_logicalDevice(_logicalDevice)
{
    std::vector<char> shaderCode = readShaderBinaryFile(_filename);
    VKNGINE_LOG_INFO("Load '" << _filename << "' " << shaderCode.size() << " byte");

    VkShaderModuleCreateInfo shaderModuleCreateInfo{};
    shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCreateInfo.codeSize = shaderCode.size();
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    if(vkCreateShaderModule(_logicalDevice.get(), &shaderModuleCreateInfo, nullptr, &m_shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create the shader module");
    }
}

ShaderModule::~ShaderModule() { vkDestroyShaderModule(m_logicalDevice.get(), m_shaderModule, nullptr); }
