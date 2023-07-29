#ifndef EXPGE_COMBINENODE_H
#define EXPGE_COMBINENODE_H

#include "Node.h"

#include <fmt/core.h>
#include <array>

namespace OmbrageNodes {

    class CombineNode : public Node{
    public:
        CombineNode(uint32_t id);
        ~CombineNode() = default;

        std::string GetNodeFunctionName() override;
        uint32_t GetNodeIDOffset() override { return 5; }

        bool RenderProperties() override;

    private:
        void UpdateType();

        NodePinTypes m_outType;

        // Properties
        int m_comboIDX;
    };

} // OmbrageNodes

#endif //EXPGE_COMBINENODE_H
