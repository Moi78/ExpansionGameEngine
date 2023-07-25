#include "RD_ShaderLoader.h"

std::vector<char> ReadFileBin(std::string filename) {
    std::ifstream f(filename, std::ios::ate | std::ios::binary);

    if(!f.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return {};
    }

    size_t fSize = (size_t)f.tellg();
    std::vector<char> buff(fSize);

    f.seekg(0);
    f.read(buff.data(), fSize);
    f.close();

    return buff;
}

#ifdef BUILD_VULKAN

RD_ShaderLoader_Vk::RD_ShaderLoader_Vk(VkDevice dev) {
    m_dev = dev;
}

RD_ShaderLoader_Vk::~RD_ShaderLoader_Vk() {
    vkDestroyShaderModule(m_dev, m_vert, nullptr);
    vkDestroyShaderModule(m_dev, m_frag, nullptr);
}

bool RD_ShaderLoader_Vk::CompileShaderFromFile(std::string vert, std::string frag) {
    auto vFile = ReadFileBin(vert);
    auto fFile = ReadFileBin(frag);

    //VERTEX
    VkShaderModuleCreateInfo vcInfo{};
    vcInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vcInfo.codeSize = vFile.size();
    vcInfo.pCode = reinterpret_cast<const uint32_t*>(vFile.data());

    if(vkCreateShaderModule(m_dev, &vcInfo, nullptr, &m_vert) != VK_SUCCESS) {
        std::cerr << "Failed to load " << vert << std::endl;
        return false;
    }

    //FRAGMENT
    VkShaderModuleCreateInfo fcInfo{};
    fcInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fcInfo.codeSize = fFile.size();
    fcInfo.pCode = reinterpret_cast<const uint32_t*>(fFile.data());

    if(vkCreateShaderModule(m_dev, &fcInfo, nullptr, &m_frag) != VK_SUCCESS) {
        std::cerr << "Failed to load " << frag << std::endl;
        return false;
    }

    return true;
}

bool RD_ShaderLoader_Vk::LoadFragBinary(std::string vert, std::vector<uint32_t> bin) {
    auto vFile = ReadFileBin(vert);

    //VERTEX
    VkShaderModuleCreateInfo vcInfo{};
    vcInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vcInfo.codeSize = vFile.size();
    vcInfo.pCode = reinterpret_cast<const uint32_t*>(vFile.data());

    if(vkCreateShaderModule(m_dev, &vcInfo, nullptr, &m_vert) != VK_SUCCESS) {
        std::cerr << "Failed to load " << vert << std::endl;
        return false;
    }

    //FRAGMENT
    VkShaderModuleCreateInfo fcInfo{};
    fcInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    fcInfo.codeSize = bin.size() * sizeof(uint32_t);
    fcInfo.pCode = reinterpret_cast<const uint32_t*>(bin.data());

    if(vkCreateShaderModule(m_dev, &fcInfo, nullptr, &m_frag) != VK_SUCCESS) {
        std::cerr << "ERROR: Failed to load binary fragment shader" << std::endl;
        return false;
    }

    return true;
}

bool RD_ShaderLoader_Vk::CompileShaderFromCode(std::string vert, std::string frag) {
    return true;
}

VkShaderModule RD_ShaderLoader_Vk::GetVertShaderModule() {
    return m_vert;
}

VkShaderModule RD_ShaderLoader_Vk::GetFragShaderModule() {
    return m_frag;
}

#endif //BUILD_VULKAN

