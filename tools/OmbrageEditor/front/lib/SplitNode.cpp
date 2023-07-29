#include "SplitNode.h"

namespace OmbrageNodes {
    SplitNode::SplitNode(uint32_t id) : Node(id) {
        m_io = {
                {NodePinMode::INPUT, NodePinTypes::OMNI, "Vector Input", 0},
        };

        m_nodeName = "Split Vector";

        m_color[0] = 35.0f;
        m_color[1] = 109.0f;
        m_color[2] = 207.0f;
    }

    bool SplitNode::ConnectPins(std::shared_ptr<Node> other, uint32_t src, uint32_t dest, uint32_t linkID) {
        bool res = Node::ConnectPins(other, src, dest, linkID);
        if(!res) {
            return false;
        }

        return LinkPostCheck(other, src, dest);
    }

    bool SplitNode::LinkPostCheck(std::shared_ptr<Node> other, uint32_t src, uint32_t dest) {
        NodePinTypes otherType = other->GetPinType(src);

        bool isVec = otherType == NodePinTypes::VEC4;
        isVec |= otherType == NodePinTypes::VEC3;
        isVec |= otherType == NodePinTypes::VEC2;

        if(isVec) {
            int compType = 0;
            std::array<std::string, 4> letterLUT = {"X", "Y", "Z", "W"};

            m_io.clear();
            m_io.push_back({NodePinMode::INPUT, otherType, "Vector Input", 0});

            if(otherType == NodePinTypes::VEC4) {
                compType = 4;
            } else if(otherType == NodePinTypes::VEC3) {
                compType = 3;
            } else if(otherType == NodePinTypes::VEC2) {
                compType = 2;
            }

            for(int i = 0; i < compType; i++) {
                m_io.push_back({NodePinMode::OUTPUT, NodePinTypes::FLOAT, letterLUT[i], (uint32_t)i + 1});
            }

            return true;
        } else {
            return false;
        }
    }

    std::string SplitNode::GetNodeFunctionName() {
        std::string suffix = GetTypeSuffix(m_io[0].type);

        return "vector/split" + suffix;
    }

    std::shared_ptr<SpOpFunCall> SplitNode::MakeFunctionCall(
            std::shared_ptr<SpirvCompiler> compiler,
            std::string funcName,
            std::vector<std::shared_ptr<SpirvDataWrapperBase>> args,
            int outID
    ) {
        int idx = outID - m_id - 1;
        auto ctant = std::make_shared<SPVIntConstant>();
        ctant->data = idx;
        ctant->type = HLTypes::INT;
        compiler->RegisterConstant(ctant);

        auto wrapped_ctant = compiler->GetConstantW(ctant);
        args.push_back(wrapped_ctant);

        auto function = compiler->GetFunction(funcName);
        auto funCall = std::make_shared<SpOpFunCall>(function, args, true);
        compiler->AddFunCallToEntry(funCall);

        return funCall;
    }
} // OmbrageNodes