#ifndef _RD_SHADER_LOADER
#define _RD_SHADER_LOADER

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<char> ReadFileBin(std::string filename);

class RD_ShaderLoader {
public:
    RD_ShaderLoader() {};
    virtual ~RD_ShaderLoader() {}

    virtual bool CompileShaderFromFile(std::string vert, std::string frag) = 0;
    virtual bool CompileShaderFromCode(std::string vert, std::string frag) = 0;
    virtual bool LoadFragBinary(std::string vert, std::vector<uint32_t> bin) = 0;
};

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

class RD_ShaderLoader_Vk : public RD_ShaderLoader {
public:
    RD_ShaderLoader_Vk(VkDevice dev);
    virtual ~RD_ShaderLoader_Vk();

    bool CompileShaderFromFile(std::string vert, std::string frag) override;
    bool CompileShaderFromCode(std::string vert, std::string frag) override;
    bool LoadFragBinary(std::string vert, std::vector<uint32_t> bin) override;

    VkShaderModule GetVertShaderModule();
    VkShaderModule GetFragShaderModule();

private:
    VkDevice m_dev;
    VkShaderModule m_vert, m_frag;
};

#endif //BUILD_VULKAN

#endif //_RD_SHADER_LOADER
