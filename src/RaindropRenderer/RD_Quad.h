#ifndef EXPGE_RD_QUAD_H
#define EXPGE_RD_QUAD_H

#include <vec.h>

#include "RD_API.h"
#include "RD_VertexBuffer.h"

class RD_Quad {
public:
    RD_Quad(std::shared_ptr<RD_API> api);
    ~RD_Quad();

    std::shared_ptr<RD_IndexedVertexBuffer> GetVertexBuffer();

private:
    std::shared_ptr<RD_IndexedVertexBuffer> m_vbuff;
};

#endif //EXPGE_RD_QUAD_H
