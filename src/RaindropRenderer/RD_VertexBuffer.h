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

class RD_IndexedVertexBuffer {
public:
    RD_IndexedVertexBuffer() {};
    virtual ~RD_IndexedVertexBuffer() {};

    virtual bool FillBufferData(std::vector<float>& vertexData, std::vector<uint32_t> indexData) = 0;
    virtual size_t GetBufferSize() = 0;

    virtual uint32_t GetNumberOfIndices() = 0;
};

std::vector<float> MakeVertexData(
        std::vector<vec3> pos,
        std::vector<vec3> norm,
        std::vector<vec2> uv,
        std::vector<vec3> tan,
        std::vector<vec3> bitan,
        std::vector<vec4> weights,
        std::vector<vec4> bonesID
);

#ifdef BUILD_VULKAN

#include <vulkan/vulkan.hpp>

class RD_VertexBuffer_Vk : public RD_VertexBuffer {
public:
    RD_VertexBuffer_Vk(VkDevice dev, VkPhysicalDevice pdev, VkQueue gfxQueue, VkCommandPool cmdPool);
    ~RD_VertexBuffer_Vk() override;

    bool FillBufferData(std::vector<float>& vertexData) override;
    size_t GetBufferSize() override;

    virtual void BindBuffer(VkCommandBuffer cmd);

private:
    std::unique_ptr<RD_Buffer_Vk> m_buffer;
    std::unique_ptr<RD_Buffer_Vk> m_stagingBuffer;
};

class RD_IndexedVertexBuffer_Vk : public RD_IndexedVertexBuffer, RD_VertexBuffer_Vk {
public:
    RD_IndexedVertexBuffer_Vk(VkDevice dev, VkPhysicalDevice pdev, VkQueue gfxQueue, VkCommandPool cmdPool);
    ~RD_IndexedVertexBuffer_Vk() override;

    bool FillBufferData(std::vector<float>& vertexData, std::vector<uint32_t> indexData) override;
    size_t GetBufferSize() override;
    uint32_t GetNumberOfIndices() override;

    void BindBuffer(VkCommandBuffer cmd) override;

private:
    std::unique_ptr<RD_Buffer_Vk> m_indBuffer;
    std::unique_ptr<RD_Buffer_Vk> m_stagingIndBuffer;

    uint32_t m_nbrInd;
};

#endif //BUILD_VULKAN

#endif //EXPGE_RD_VERTEXBUFFER_H
