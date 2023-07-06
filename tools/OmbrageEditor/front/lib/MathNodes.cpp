#include "MathNodes.h"

bool OmbrageNodes::MathNode::LinkPostCheck(std::shared_ptr<Node> other, uint32_t src, uint32_t dest) {
    NodePinTypes otherType = other->GetPinType(src);
    NodePinTypes compType;

    int locID = dest - m_id;
    if(locID == 0) {
        compType = m_io[1].type;
    } else {
        compType = m_io[0].type;
    }

    if(!TypeCompatibility(otherType, compType)) {
        m_connections.pop_back();
        return false;
    }

    m_io[locID].type = otherType;

    if(TypeWeightCompare(otherType, compType)) {
        m_io[2].type = otherType;
    } else {
        m_io[2].type = compType;
    }

    return true;
}

bool OmbrageNodes::MathNode::ConnectPins(std::shared_ptr<Node> other, uint32_t srcID, uint32_t pID, uint32_t linkID) {
    bool res = Node::ConnectPins(other, srcID, pID, linkID);
    if(!res) {
        return false;
    }

    return LinkPostCheck(other, srcID, pID);
}

void OmbrageNodes::MathNode::ValidateLinks() {
    std::vector<std::vector<NodeConnection>::iterator> toDelete;

    std::vector<NodeConnection>::iterator it = m_connections.begin();
    for(auto& con : m_connections) {
        NodePinTypes tother = con.OtherNode->GetPinType(con.src_pinID);
        NodePinTypes tself = GetPinType(con.dst_pinID);

        if(tother != tself) {
            toDelete.push_back(it);

            int locID = con.dst_pinID - m_id;
            m_io[locID].type = NodePinTypes::OMNI;
            m_io[2].type = locID == 1 ? m_io[0].type : m_io[1].type;
        }

        it++;
    }

    for(auto del : toDelete) {
        m_connections.erase(del);
    }
}

// ----------------------------------------------------------------------------------------

OmbrageNodes::ConstFloatNode::ConstFloatNode(uint32_t id) : Node(id) {
    m_io = {{ NodePinMode::OUTPUT, NodePinTypes::FLOAT, "Const 0.0", 0 }};
    m_nodeName = "Const Float";

    m_float = 0.0f;
}

void OmbrageNodes::ConstFloatNode::RenderProperties() {
    if(ImGui::CollapsingHeader("Const float")) {
        if(ImGui::InputFloat(fmt::format("Value##{}", m_id).c_str(), &m_float)) {
            m_io[0].pinName = fmt::format("Const {:.2f}", m_float);
        }
    }
}

OmbrageNodes::ConstVec4Node::ConstVec4Node(uint32_t id) : Node(id) {
    m_io = {{ NodePinMode::OUTPUT, NodePinTypes::VEC4, "Const 0.0 0.0 0.0 0.0", 0 }};
    m_nodeName = "Const Vec4";

    for(auto& v : m_vec4) {
        v = 0.0f;
    }
}

void OmbrageNodes::ConstVec4Node::RenderProperties() {
    if(ImGui::CollapsingHeader("Const Vec 4")) {
        if(
                ImGui::InputFloat4(fmt::format("Value##{}", m_id).c_str(), m_vec4) ||
                ImGui::ColorEdit4(fmt::format("Color##{}", m_id).c_str(), m_vec4, ImGuiColorEditFlags_NoInputs)
        ) {
            m_io[0].pinName = fmt::format("Const {:0.2f} {:0.2f} {:0.2f} {:0.2f}", m_vec4[0], m_vec4[1], m_vec4[2], m_vec4[3]);

            for(int i = 0; i < 3; i++) {
                m_color[i] = m_vec4[i] * 255.0f;
            }
        }
    }
}

OmbrageNodes::ConstVec3Node::ConstVec3Node(uint32_t id) : Node(id) {
    m_io = {{ NodePinMode::OUTPUT, NodePinTypes::VEC3, "Const 0.0 0.0 0.0", 0 }};
    m_nodeName = "Const Vec3";

    for(auto& v : m_vec3) {
        v = 0.0f;
    }
}

void OmbrageNodes::ConstVec3Node::RenderProperties() {
    if(ImGui::CollapsingHeader("Const Vec 3")) {
        if(
                ImGui::InputFloat3(fmt::format("Value##{}", m_id).c_str(), m_vec3) ||
                ImGui::ColorEdit3(fmt::format("Color##{}", m_id).c_str(), m_vec3, ImGuiColorEditFlags_NoInputs)
        ) {
            m_io[0].pinName = fmt::format("Const {:0.2f} {:0.2f} {:0.2f}", m_vec3[0], m_vec3[1], m_vec3[2]);

            for(int i = 0; i < 3; i++) {
                m_color[i] = m_vec3[i] * 255.0f;
            }
        }
    }
}

OmbrageNodes::ConstVec2Node::ConstVec2Node(uint32_t id) : Node(id) {
    m_io = {{ NodePinMode::OUTPUT, NodePinTypes::VEC2, "Const 0.0 0.0", 0 }};
    m_nodeName = "Const Vec2";

    for(auto& v : m_vec2) {
        v = 0.0f;
    }
}

void OmbrageNodes::ConstVec2Node::RenderProperties() {
    if(ImGui::CollapsingHeader("Const Vec 2")) {
        if(ImGui::InputFloat2(fmt::format("Value##{}", m_id).c_str(), m_vec2)) {
            m_io[0].pinName = fmt::format("Const {:0.2f} {:0.2f}", m_vec2[0], m_vec2[1]);
        }
    }
}

// ----------------------------------------------------------------------------------------

OmbrageNodes::AddNode::AddNode(uint32_t id) : MathNode(id) {
    m_io = {
            { NodePinMode::INPUT,  NodePinTypes::OMNI, "a", 0 },
            { NodePinMode::INPUT,  NodePinTypes::OMNI, "b", 1 },
            { NodePinMode::OUTPUT,  NodePinTypes::OMNI, "Result", 2 }
    };

    m_nodeName = "Add";
}

// -----------------------------------------------------------------------------------------

OmbrageNodes::MultNode::MultNode(uint32_t id) : MathNode(id) {
    m_io = {
            { NodePinMode::INPUT,  NodePinTypes::OMNI, "a", 0 },
            { NodePinMode::INPUT,  NodePinTypes::OMNI, "b", 1 },
            { NodePinMode::OUTPUT,  NodePinTypes::OMNI, "Result", 2 }
    };

    m_nodeName = "Multiply";
}