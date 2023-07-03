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
    m_io = {{ NodePinMode::OUTPUT, NodePinTypes::FLOAT, "Const", 0 }};

    m_nodeName = "Const Float";
}

OmbrageNodes::ConstVec4Node::ConstVec4Node(uint32_t id) : Node(id) {
    m_io = {{ NodePinMode::OUTPUT, NodePinTypes::VEC4, "Const", 0 }};

    m_nodeName = "Const Vec4";
}

OmbrageNodes::ConstVec3Node::ConstVec3Node(uint32_t id) : Node(id) {
    m_io = {{ NodePinMode::OUTPUT, NodePinTypes::VEC3, "Const", 0 }};

    m_nodeName = "Const Vec3";
}

OmbrageNodes::ConstVec2Node::ConstVec2Node(uint32_t id) : Node(id) {
    m_io = {{ NodePinMode::OUTPUT, NodePinTypes::VEC2, "Const", 0 }};

    m_nodeName = "Const Vec2";
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
