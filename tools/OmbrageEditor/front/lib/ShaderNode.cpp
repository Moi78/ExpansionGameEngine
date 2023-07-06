#include "ShaderNode.h"

ShaderNode::ShaderNode(uint32_t id) : Node(id) {
    m_io = {
            { NodePinMode::INPUT, NodePinTypes::VEC3, "Color", 0 },
            { NodePinMode::INPUT, NodePinTypes::VEC3, "Normal", 1 },
            { NodePinMode::INPUT, NodePinTypes::VEC3, "Pos", 2 },
            { NodePinMode::INPUT, NodePinTypes::FLOAT, "Metallic", 3 },
            { NodePinMode::INPUT, NodePinTypes::FLOAT, "Roughness", 4 },
            { NodePinMode::INPUT, NodePinTypes::FLOAT, "AO", 5 },
            { NodePinMode::INPUT, NodePinTypes::VEC4, "Sheen", 6 }
    };

    m_nodeName = "Shader Node";
    m_color[1] = 190.0f;
}