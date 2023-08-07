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
            return "f"; // Defaulting to FLOAT when an omni is not defined
            break;
    }
}

NodeConnection_Serializable NodeConnection::ToSerializable() const {
    uint32_t otherID = OtherNode.lock()->GetNodeID();

    return {otherID, dst_pinID, src_pinID, linkID};
}

// -----------------------------------------------------

Node::Node(uint32_t id) : m_id(id) {
    for(auto& c : m_color) {
        c = 0.0f;
    }

    m_isConst = false;
    m_isShaderIn = false;
    m_holdTexture = false;
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
        if(!con.OtherNode.use_count()) {
            toDelete.push_back(it);
        } else {
            auto othern = std::shared_ptr<Node>(con.OtherNode);
            NodePinTypes tother = othern->GetPinType(con.src_pinID);
            NodePinTypes tself = GetPinType(con.dst_pinID);

            if (tother != tself) {
                toDelete.push_back(it);
            }

            it++;
        }
    }

    for(auto del : toDelete) {
        m_connections.erase(del);
    }
}

std::shared_ptr<Node> Node::GetParent(int pinID) {
    for(auto& c : m_connections) {
        if(c.dst_pinID == pinID) {
            return std::shared_ptr<Node>(c.OtherNode);
        }
    }

    return {};
}

std::shared_ptr<FuncGraphElem> Node::EvalFrom(int locID, std::shared_ptr<SpirvCompiler> compiler) {
    std::shared_ptr<Node> parent = GetParent(m_id + locID);
    if(!parent.use_count()) {
        auto noneFGE = std::make_shared<FuncGraphElem>();
        noneFGE->t = FuncGraphElemType::NONE;

        return noneFGE;
    }

    uint32_t par_id = parent->GetNodeID();

    if(parent->isConst()) {
        auto ctant = std::dynamic_pointer_cast<Constant>(parent);

        auto elem = std::make_shared<HighLevelCtant>();
        elem->ctant = compiler->GetConstantW(ctant->ctant);

        return elem;

    } else if(parent->isShaderIn()) {
        auto in_id = std::dynamic_pointer_cast<ShaderInWrapper>(parent);

        auto elem = std::make_shared<HighLevelCtant>();
        elem->ctant = compiler->GetInputVariableW(in_id->shaderVarID);

        return elem;
    } else {
        auto elem = std::make_unique<HighLevelFunCall>();
        elem->func = parent->GetNodeFunctionName();

        std::vector<std::shared_ptr<SpirvDataWrapperBase>> wraps;
        for(int i = 0; i < parent->GetNodeSize(); i++) {
            if(parent->GetPinMode(par_id + i) != NodePinMode::OUTPUT) {
                std::shared_ptr<FuncGraphElem> evaluated = parent->EvalFrom(i, compiler);

                if(evaluated->t == FuncGraphElemType::FUNCTION) {
                    auto funcnode = std::reinterpret_pointer_cast<HighLevelFunCall>(evaluated);

                    std::shared_ptr<SpirvDataWrapperFunRet> ret = std::make_shared<SpirvDataWrapperFunRet>();
                    ret->fcall = funcnode->funCall;

                    wraps.push_back(ret);
                } else if(evaluated->t == FuncGraphElemType::CONST) {
                    auto ctantnode = std::reinterpret_pointer_cast<HighLevelCtant>(evaluated);
                    wraps.push_back(ctantnode->ctant);
                } else if(evaluated->t == FuncGraphElemType::NONE) {
                    auto default_value = GetDefaultValue(compiler, parent->GetPinType(par_id + i));
                    wraps.push_back(default_value);
                }

                elem->parents.push_back(std::move(evaluated));
            }
        }

        int outID = -1;
        for(auto& link : m_connections) {
            if(link.dst_pinID == (locID + m_id)) {
                outID = link.src_pinID;
            }
        }

        elem->funCall = parent->MakeFunctionCall(compiler, elem->func, wraps, outID);

        return elem;
    }
}

NodePinMode Node::GetPinMode(uint32_t globID) {
    return m_io[globID - m_id].mode;
}

std::shared_ptr<SpOpFunCall> Node::MakeFunctionCall(
        std::shared_ptr<SpirvCompiler> compiler,
        std::string funcName,
        std::vector<std::shared_ptr<SpirvDataWrapperBase>> args,
        int outID
) {
    auto function = compiler->GetFunction(funcName);

    std::shared_ptr<SpOpFunCall> fcall = std::make_shared<SpOpFunCall>(function, args, true);
    compiler->AddFunCallToEntry(fcall);

    return fcall;
}

std::shared_ptr<SpirvDataWrapperBase> Node::GetDefaultValue(
        std::shared_ptr<SpirvCompiler> compiler,
        NodePinTypes type
) {
    std::shared_ptr<SpirvConstant> ctant;
    std::shared_ptr<SpirvDataWrapperBase> zero_wrapper;

    if(type == NodePinTypes::OMNI || type == NodePinTypes::FLOAT) {
        ctant = std::make_shared<SPVFloatConstant>();
        ctant->type = HLTypes::FLOAT;

    } else if(type == NodePinTypes::VEC2) {
        ctant = std::make_shared<SPVVecConstant<2>>();
        ctant->type = HLTypes::VECTOR2;

    } else if(type == NodePinTypes::VEC3) {
        ctant = std::make_shared<SPVVecConstant<3>>();
        ctant->type = HLTypes::VECTOR3;

    } else if(type == NodePinTypes::VEC4) {
        ctant = std::make_shared<SPVVecConstant<4>>();
        ctant->type = HLTypes::VECTOR4;

    } else if(type == NodePinTypes::INT) {
        ctant = std::make_shared<SPVIntConstant>();
        ctant->type = HLTypes::INT;

    }

    compiler->RegisterConstant(ctant);
    zero_wrapper = compiler->GetConstantW(ctant);

    return zero_wrapper;
}

bool Node::ContainsLink(int linkID) {
    for(auto& l : m_connections) {
        if(l.linkID == linkID) {
            return true;
        }
    }

    return false;
}

void Node::DeleteLink(int linkID) {
    std::vector<int> deleteInd;
    for(int i = 0; i < m_connections.size(); i++) {
        if(m_connections[i].linkID == linkID) {
            m_connections.erase(m_connections.begin() + i);
            break;
        }
    }
}

void Node::Serialize(std::ofstream &bin_stream) {
    // Node name
    uint32_t nameSize = m_nodeName.size();

    bin_stream.write(reinterpret_cast<char*>(&nameSize), sizeof(uint32_t));
    bin_stream.write(m_nodeName.c_str(), m_nodeName.size());

    // Node ID
    bin_stream.write(reinterpret_cast<char*>(&m_id), sizeof(uint32_t));

    // Links
    uint32_t conSize = m_connections.size();
    bin_stream.write(reinterpret_cast<char*>(&conSize), sizeof(uint32_t));

    for(auto& l : m_connections) {
        NodeConnection_Serializable ser = l.ToSerializable();
        bin_stream.write(reinterpret_cast<char*>(&ser), sizeof(NodeConnection_Serializable));
    }

    // Node Pos
    ImVec2 pos = ImNodes::GetNodeEditorSpacePos(m_id);
    bin_stream.write(reinterpret_cast<char*>(&pos), sizeof(ImVec2));

    // Properties
    std::vector<char> prop = SerializeProperties();
    uint32_t propSize = prop.size();

    bin_stream.write(reinterpret_cast<char*>(&propSize), sizeof(uint32_t));
    bin_stream.write(prop.data(), prop.size());
}

void Node::SetNodePos(ImVec2 pos) {
    ImNodes::SetNodeEditorSpacePos(m_id, pos);
}

void Node::LoadLinks(std::vector<NodeConnection> links) {
    m_connections.clear();
    m_connections = links;
}
