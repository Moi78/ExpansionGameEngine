#ifndef EXPGE_SHADERINNODE_H
#define EXPGE_SHADERINNODE_H

#include "Node.h"

namespace OmbrageNodes {

    class NormalNode : public Node, public ShaderInWrapper {
    public:
        NormalNode(uint32_t id);
        ~NormalNode() = default;

        std::string GetNodeFunctionName() override { return "ctant"; }
    };

    class PosNode : public Node, public ShaderInWrapper {
    public:
        PosNode(uint32_t id);
        ~PosNode() = default;

        std::string GetNodeFunctionName() override { return "ctant"; }
    };

    class UVNode : public Node, public ShaderInWrapper {
    public:
        UVNode(uint32_t id);
        ~UVNode() = default;

        std::string GetNodeFunctionName() override { return "ctant";}
    };

    class TanNode : public Node, public ShaderInWrapper {
    public:
        TanNode(uint32_t id);
        ~TanNode() = default;

        std::string GetNodeFunctionName() override { return "ctant"; }
    };

    class BitanNode : public Node, public ShaderInWrapper {
    public:
        BitanNode(uint32_t id);
        ~BitanNode() = default;

        std::string GetNodeFunctionName() override { return "ctant"; }
    };
}

#endif //EXPGE_SHADERINNODE_H
