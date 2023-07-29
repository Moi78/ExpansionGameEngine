#ifndef EXPGE_SPLITNODE_H
#define EXPGE_SPLITNODE_H

#include "Node.h"

#include <fmt/core.h>
#include <array>

namespace OmbrageNodes {

    class SplitNode : public Node {
    public:
        SplitNode(uint32_t id);
        ~SplitNode() = default;

        bool LinkPostCheck(std::shared_ptr<Node> other, uint32_t src, uint32_t dest);
        bool ConnectPins(std::shared_ptr<Node> other, uint32_t src, uint32_t dest, uint32_t linkID) override;

        std::string GetNodeFunctionName() override;
        std::shared_ptr<SpOpFunCall> MakeFunctionCall(std::shared_ptr<SpirvCompiler> compiler, std::string funcName, std::vector<std::shared_ptr<SpirvDataWrapperBase>> args, int outID) override;

        uint32_t GetNodeIDOffset() override { return 5; }

    private:
        std::string m_inputTypeUI;
    };

} // OmbrageNodes

#endif //EXPGE_SPLITNODE_H
