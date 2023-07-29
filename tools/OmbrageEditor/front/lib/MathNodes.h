#ifndef EXPGE_MATHNODES_H
#define EXPGE_MATHNODES_H

#include "Node.h"

#include <iostream>
#include <fmt/core.h>
#include <string>
#include <iterator>

namespace OmbrageNodes {
    class ConstFloatNode : public Node, public Constant {
    public:
        ConstFloatNode(uint32_t id);

        bool RenderProperties() override;

        std::string GetNodeFunctionName() override { return "ctant"; }

        void MakeCtant() override;
    private:
        float m_float;
    };

    class ConstVec4Node : public Node, public Constant {
    public:
        ConstVec4Node(uint32_t id);

        bool RenderProperties() override;

        std::string GetNodeFunctionName() override { return "ctant"; }

        void MakeCtant() override;
    private:
        float m_vec4[4];
    };

    class ConstVec3Node : public Node, public Constant {
    public:
        ConstVec3Node(uint32_t id);

        bool RenderProperties() override;

        std::string GetNodeFunctionName() override { return "ctant"; }

        void MakeCtant() override;
    private:
        float m_vec3[3];
    };

    class ConstVec2Node : public Node, public Constant {
    public:
        ConstVec2Node(uint32_t id);

        bool RenderProperties() override;

        std::string GetNodeFunctionName() override { return "ctant"; }

        void MakeCtant() override;
    private:
        float m_vec2[2];
    };

    class MathNode : public Node {
    public:
        MathNode(uint32_t id) : Node(id) {
            m_color[0] = 255.0;
        }

        bool LinkPostCheck(std::shared_ptr<Node> other, uint32_t src, uint32_t dest);
        bool ConnectPins(std::shared_ptr<Node> other, uint32_t srcID, uint32_t pID, uint32_t linkID) override;

        void ValidateLinks() override;
        std::shared_ptr<FuncGraphElem> EvalFrom(int locID, std::shared_ptr<SpirvCompiler> compiler) override;
    };

    class AddNode : public MathNode {
    public:
        AddNode(uint32_t id);

        std::string GetNodeFunctionName() override;
    };

    class MultNode : public MathNode {
    public:
        MultNode(uint32_t id);

        std::string GetNodeFunctionName() override;
    };
}


#endif //EXPGE_MATHNODES_H
