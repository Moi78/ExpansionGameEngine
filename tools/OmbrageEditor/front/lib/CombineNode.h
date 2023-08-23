#ifndef EXPGE_COMBINENODE_H
#define EXPGE_COMBINENODE_H

#include "Node.h"

#include <fmt/core.h>
#include <array>

namespace OmbrageNodes {

    struct CombineProperties {
        NodePinTypes outType;
        int comboIDX;
    };

    class CombineNode : public Node{
    public:
        CombineNode(uint32_t id);
        ~CombineNode() = default;

        std::string GetNodeFunctionName() override;
        uint32_t GetNodeIDOffset() override { return 5; }

        bool RenderProperties() override;
        std::vector<char> SerializeProperties() override;
        void LoadProperties(std::vector<char> data) override;

    private:
        void UpdateType();

        CombineProperties m_properties;
    };

} // OmbrageNodes

#endif //EXPGE_COMBINENODE_H
