#ifndef EXPGE_SHADERNODE_H
#define EXPGE_SHADERNODE_H

#include "imnodes.h"
#include "imgui.h"

#include "Node.h"

class ShaderNode : public Node {
public:
    void RenderNode() override;

    void ConnectTo(int src, int dst) override {};
};


#endif //EXPGE_SHADERNODE_H
