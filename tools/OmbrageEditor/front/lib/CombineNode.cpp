#include "CombineNode.h"

namespace OmbrageNodes {
    CombineNode::CombineNode(uint32_t id) : Node(id) {
        m_io = {
                {NodePinMode::OUTPUT, NodePinTypes::VEC3, "Vector Output", 0}
        };

        m_nodeName = "Combine";

        std::array<std::string, 3> letter_lut = {"X", "Y", "Z"};
        for(int i = 0; i < 3; i++) {
            m_io.push_back({NodePinMode::INPUT, NodePinTypes::FLOAT, letter_lut[i], (uint32_t)i + 1});
        }

        m_outType = NodePinTypes::VEC3;
        m_comboIDX = 1;

        m_color[0] = 35.0f;
        m_color[1] = 109.0f;
        m_color[2] = 207.0f;
    }

    std::string CombineNode::GetNodeFunctionName() {
        return fmt::format("vector/combine{}", GetTypeSuffix(m_outType));
    }

    bool CombineNode::RenderProperties() {
        if(ImGui::CollapsingHeader(fmt::format("Combine##{}", m_id).c_str())) {
            if(ImGui::Combo(fmt::format("Output type##{}", m_id).c_str(), &m_comboIDX, "Vector 2\0Vector 3\0Vector 4\0\0")) {
                std::array<NodePinTypes, 3> typeChoice = {NodePinTypes::VEC2, NodePinTypes::VEC3, NodePinTypes::VEC4};
                m_outType = typeChoice[m_comboIDX];

                UpdateType();
                return true;
            }
        }

        return false;
    }

    void CombineNode::UpdateType() {
        m_io.clear();
        m_io.push_back({NodePinMode::OUTPUT, m_outType, "Vector Output", 0});

        int compCount = 0;
        switch (m_outType) {
            case NodePinTypes::VEC2:
                compCount = 2;
                break;
            case NodePinTypes::VEC3:
                compCount = 3;
                break;
            case NodePinTypes::VEC4:
                compCount = 4;
                break;
            default:
                compCount = 0;
                break;
        }

        std::array<std::string, 4> letter_lut{"X", "Y", "Z", "W"};
        for(int i = 0; i < compCount; i++) {
            m_io.push_back({NodePinMode::INPUT, NodePinTypes::FLOAT, letter_lut[i], (uint32_t)i + 1});
        }

        // Link sanitazation
        std::vector<int> linkGarbage;

        int i = 0;
        for(auto& link : m_connections) {
            if(link.dst_pinID > m_id + compCount) {
                linkGarbage.push_back(i);
            }

            i++;
        }

        for(auto& link : linkGarbage) {
            m_connections.erase(m_connections.begin() + link);
        }
    }
} // OmbrageNodes