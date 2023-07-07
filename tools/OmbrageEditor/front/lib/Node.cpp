#include "Node.h"

ImVec4 MapTypeToColor(NodePinTypes t) {
    static std::unordered_map<NodePinTypes, ImVec4> colMap = {
            { NodePinTypes::INT, {0.0f, 0.859f, 1.0f, 1.0f} },
            { NodePinTypes::FLOAT, {0.051f, 0.769f, 0.051f, 1.0f} },
            { NodePinTypes::OMNI, {1.0f, 1.0f, 1.0f, 1.0f} },
            { NodePinTypes::VEC2, {1.0f, 0.0f, 0.0f, 1.0f} },
            { NodePinTypes::VEC3, {1.0f, 0.384f, 0.0f, 1.0f} },
            { NodePinTypes::VEC4, {1.0f, 1.0f, 0.0f, 1.0f} }
    };

    return colMap[t];
}

bool TypeWeightCompare(NodePinTypes a, NodePinTypes b) {
    static std::unordered_map<NodePinTypes, int> weight = {
            { NodePinTypes::INT, 1 },
            { NodePinTypes::FLOAT, 2 },
            { NodePinTypes::OMNI, 0 },
            { NodePinTypes::VEC2, 3 },
            { NodePinTypes::VEC3, 3 },
            { NodePinTypes::VEC4, 3 }
    };

    return weight[a] >= weight[b];
}

bool TypeCompatibility(NodePinTypes a, NodePinTypes b) {
    bool AisVec = (a == NodePinTypes::VEC2) || (a == NodePinTypes::VEC3) || (a == NodePinTypes::VEC4);
    bool BisVec = (b == NodePinTypes::VEC2) || (b == NodePinTypes::VEC3) || (b == NodePinTypes::VEC4);

    bool typaA_is_typeB = a == b;

    return (AisVec != BisVec) || typaA_is_typeB || (b == NodePinTypes::OMNI) || (a == NodePinTypes::OMNI);
}

std::string GetTypeSuffix(NodePinTypes t) {
    switch(t) {
        case INT:
            return "i";
            break;
        case FLOAT:
            return "f";
            break;
        case VEC2:
            return "2v";
            break;
        case VEC3:
            return "3v";
            break;
        case VEC4:
            return "4v";
            break;
        case OMNI:
            return "err";
            break;
    }
}

// -----------------------------------------------------

Node::Node(uint32_t id) : m_id(id) {
    for(auto& c : m_color) {
        c = 0.0f;
    }

    m_isConst = false;
}

void Node::RenderNode() {
    ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(m_color[0] / 2, m_color[1] / 2, m_color[2] / 2, 255));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(m_color[0] / 3, m_color[1] / 3, m_color[2] / 3, 255));
    ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(m_color[0] / 4, m_color[1] / 4, m_color[2] / 4, 255));

    ImNodes::BeginNode(m_id);

    ImNodes::BeginNodeTitleBar();
    ImGui::Text("%s", m_nodeName.c_str());
    ImNodes::EndNodeTitleBar();

    for(auto& pin : m_io) {
        ImVec4 pinColor = MapTypeToColor(pin.type);
        uint32_t pinColor_u = IM_COL32(pinColor.x * 255.0f, pinColor.y * 255.0f, pinColor.z * 255.0f, pinColor.w * 255.0f);

        ImNodes::PushColorStyle(ImNodesCol_Pin, pinColor_u);

        if(pin.mode == NodePinMode::INPUT) {
            ImNodes::BeginInputAttribute(pin.pinID + m_id);
        } else {
            ImNodes::BeginOutputAttribute(pin.pinID + m_id);
        }

        ImGui::Text("%s", pin.pinName.c_str());

        if(pin.mode == NodePinMode::INPUT) {
            ImNodes::EndInputAttribute();
        } else {
            ImNodes::EndOutputAttribute();
        }

        ImNodes::PopColorStyle();
    }

    ImNodes::EndNode();

    ImNodes::PopColorStyle();
}

void Node::RenderLinks() {
    for(auto& con : m_connections) {
        ImNodes::Link(con.linkID, con.src_pinID, con.dst_pinID);
    }
}

bool Node::ContainsPin(uint32_t globID) {
    return (globID - m_id) < m_io.size();
}

bool Node::ConnectPins(std::shared_ptr<Node> other, uint32_t srcID, uint32_t pID, uint32_t linkID) {
    if(!CheckConnectionValidity(other->GetPinType(srcID), GetPinType(pID), pID)) {
        return false;
    }

    m_connections.push_back({
        other,
        pID,
        srcID,
        linkID
    });

    return true;
}

bool Node::CheckConnectionValidity(NodePinTypes atype, NodePinTypes btype, int destPin) {
    if((atype != btype) && (atype != NodePinTypes::OMNI) && (btype != NodePinTypes::OMNI)) {
        return false;
    }

    for(auto& con : m_connections) {
        if(con.dst_pinID == destPin) {
            return false;
        }
    }

    return true;
}

NodePinTypes Node::GetPinType(uint32_t globID) {
    uint32_t locID = globID - m_id;

    for(auto& p : m_io) {
        if(p.pinID == locID) {
            return p.type;
        }
    }

    return NodePinTypes::OMNI;
}

void Node::ValidateLinks() {
    std::vector<std::vector<NodeConnection>::iterator> toDelete;

    std::vector<NodeConnection>::iterator it = m_connections.begin();
    for(auto& con : m_connections) {
        NodePinTypes tother = con.OtherNode->GetPinType(con.src_pinID);
        NodePinTypes tself = GetPinType(con.dst_pinID);

        if(tother != tself) {
            toDelete.push_back(it);
        }

        it++;
    }

    for(auto del : toDelete) {
        m_connections.erase(del);
    }
}

std::shared_ptr<Node> Node::GetParent(int pinID) {
    for(auto& c : m_connections) {
        if(c.dst_pinID == pinID) {
            return c.OtherNode;
        }
    }

    return {};
}

std::unique_ptr<FuncGraphElem> Node::EvalFrom(int locID, std::shared_ptr<SpirvCompiler> compiler) {
    std::shared_ptr<Node> parent = GetParent(m_id + locID);
    if(!parent.use_count()) {
        std::cerr << "PIN " << m_id + locID << "NOT CONNECTED" << std::endl;
        return {};
    }

    uint32_t par_id = parent->GetNodeID();

    if(!parent->isConst()) {
        auto elem = std::make_unique<HighLevelFunCall>();
        elem->func = parent->GetNodeFunctionName();

        for(int i = 0; i < parent->GetNodeSize(); i++) {
            if(parent->GetPinMode(par_id + i) != NodePinMode::OUTPUT) {
                elem->parents.push_back(parent->EvalFrom(i, compiler));
            }
        }

        return elem;
    } else {
        auto ctant = std::dynamic_pointer_cast<Constant>(parent);

        auto elem = std::make_unique<HighLevelCtant>();
        elem->ctant = compiler->GetConstantW(ctant->ctant);

        return elem;
    }
}

NodePinMode Node::GetPinMode(uint32_t globID) {
    return m_io[globID - m_id].mode;
}
