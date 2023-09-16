#include "ShaderInNode.h"

namespace OmbrageNodes {
    NormalNode::NormalNode(uint32_t id) : Node(id) {
        m_io = {{NodePinMode::OUTPUT, NodePinTypes::VEC3, "Normal Vector", 0}};

        m_nodeName = "Normal";

        m_color[0] = 252.0f;
        m_color[1] = 202.0f;
        m_color[2] = 3.0f;

        m_isShaderIn = true;

        shaderVarID = 1;
    }

    PosNode::PosNode(uint32_t id) : Node(id) {
        m_io = {{NodePinMode::OUTPUT, NodePinTypes::VEC3, "Position Vector", 0}};

        m_nodeName = "Position";

        m_color[0] = 252.0f;
        m_color[1] = 202.0f;
        m_color[2] = 3.0f;

        m_isShaderIn = true;

        shaderVarID = 2;
    }

    UVNode::UVNode(uint32_t id) : Node(id) {
        m_io = {{NodePinMode::OUTPUT, NodePinTypes::VEC2, "Coordinates", 0}};

        m_nodeName = "UV Coordinates";

        m_color[0] = 252.0f;
        m_color[1] = 202.0f;
        m_color[2] = 3.0f;

        m_isShaderIn = true;

        shaderVarID = 0;
    }

    TanNode::TanNode(uint32_t id) : Node(id) {
        m_io = {{NodePinMode::OUTPUT, NodePinTypes::VEC3, "Tangent Vector", 0}};

        m_nodeName = "Tangent";

        m_color[0] = 252.0f;
        m_color[1] = 202.0f;
        m_color[2] = 3.0f;

        m_isShaderIn = true;

        shaderVarID = 3;
    }

    BitanNode::BitanNode(uint32_t id) : Node(id) {
        m_io = {{NodePinMode::OUTPUT, NodePinTypes::VEC3, "Bi-Tangent Vector", 0}};

        m_nodeName = "Bi-Tangent";

        m_color[0] = 252.0f;
        m_color[1] = 202.0f;
        m_color[2] = 3.0f;

        m_isShaderIn = true;

        shaderVarID = 4;
    }
}