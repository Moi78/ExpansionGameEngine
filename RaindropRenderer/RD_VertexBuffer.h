#ifndef EXPGE_RD_VERTEXBUFFER_H
#define EXPGE_RD_VERTEXBUFFER_H

#include <vector>
#include <iostream>

#include "RD_Buffer.h"
#include "vec.h"

class RD_VertexBuffer {
public:
    RD_VertexBuffer() {};
    virtual ~RD_VertexBuffer() {};

    virtual bool FillBufferData(std::vector<float>& vertexData) = 0;
    virtual size_t GetBufferSize() = 0;
};

std::vector<float> MakeVertexData(std::vector<vec3> pos, std::vector<vec3> norm, std::vector<vec2> uv);

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

class RD_VertexBuffer_Vk : public RD_VertexBuffer {
public:
    RD_VertexBuffer_Vk(VkDevice dev, VkPhysicalDevice pdev, VkQueue gfxQueue, VkCommandPool cmdPool);
    ~RD_VertexBuffer_Vk() override;

    bool FillBufferData(std::vector<float>& vertexData) override;
    size_t GetBufferSize() override;

    void BindBuffer(VkCommandBuffer cmd);

private:
    std::unique_ptr<RD_Buffer_Vk> m_buffer;
    std::unique_ptr<RD_Buffer_Vk> m_stagingBuffer;
};

#endif //BUILD_VULKAN

#endif //EXPGE_RD_VERTEXBUFFER_H
