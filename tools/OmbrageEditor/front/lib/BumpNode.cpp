#include "BumpNode.h"

namespace OmbrageNodes {
    BumpNode::BumpNode(int id) : Node(id) {
        m_io = {
                {NodePinMode::INPUT, NodePinTypes::VEC4, "Normal Map", 0},
                {NodePinMode::INPUT, NodePinTypes::VEC3, "Normal Vector", 1},
                {NodePinMode::INPUT, NodePinTypes::VEC3, "Position", 2},
                {NodePinMode::INPUT, NodePinTypes::VEC2, "UV Coordinates", 3},
                {NodePinMode::OUTPUT, NodePinTypes::VEC3, "Normal", 4}
        };

        m_nodeName = "Bump";

        m_color[0] = 180.0f;
        m_color[1] = 82.0f;
        m_color[2] = 255.0f;
    }
}
