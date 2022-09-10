#ifndef EXPGE_RD_MESH_H
#define EXPGE_RD_MESH_H

#include <iostream>
#include <string>
#include <memory>

#include "RD_API.h"
#include "RD_VertexBuffer.h"

#include <BD_Reader.h>

#include <mat4.h>
#include <vec.h>

class RD_Mesh {
public:
    RD_Mesh(std::shared_ptr<RD_API> api, vec3 pos, vec3 rot, vec3 scale);
    ~RD_Mesh();

    void LoadMesh(const std::string& filepath);
    void Update();

    mat4f GetTransform();
    std::shared_ptr<RD_IndexedVertexBuffer> GetVertexBuffer();

    void Rotate(vec3 rotation);

private:
    std::shared_ptr<RD_API> m_api;
    std::shared_ptr<RD_IndexedVertexBuffer> m_vbuff;

    mat4f m_transf;
    vec3 m_pos;
    vec3 m_rot;
    vec3 m_scale;
};

#endif //EXPGE_RD_MESH_H
