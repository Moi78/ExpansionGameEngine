#ifndef EXPGE_SHADERNODE_H
#define EXPGE_SHADERNODE_H

#include "imnodes.h"
#include "imgui.h"

#include "Node.h"

class ShaderNode : public Node {
public:
    ShaderNode(uint32_t id);

    std::string GetNodeFunctionName() override { return "shadernode"; };
};


#endif //EXPGE_SHADERNODE_H
