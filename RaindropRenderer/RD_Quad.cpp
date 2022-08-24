#include "RD_Quad.h"

RD_Quad::RD_Quad(std::shared_ptr<RD_API> api) {
    m_vbuff = api->CreateIndexedVertexBuffer();

    auto vData = MakeVertexData(
            { vec3(-1.0, 1.0, 0.0), vec3(-1.0, -1.0, 0.0), vec3(1.0, 1.0, 0.0), vec3(1.0, -1.0, 0.0) },
            { vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0) },
            { vec2(0.0, 1.0), vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 0.0)}
    );

    std::vector<uint32_t> indices = {
            0, 1, 2,
            1, 3, 2
    };

    m_vbuff->FillBufferData(vData, indices);
}

RD_Quad::~RD_Quad() {

}

std::shared_ptr<RD_IndexedVertexBuffer> RD_Quad::GetVertexBuffer() {
    return m_vbuff;
}