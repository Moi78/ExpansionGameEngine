#include "NodeGraph_UI.h"
#include "ShaderNode.h"

namespace OmbrageUI {
    NodeGraph_UI::NodeGraph_UI() {
        m_linkCount = 0;
        m_next_id = 0;
    }

    NodeGraph_UI::~NodeGraph_UI() {

    }

    void NodeGraph_UI::AddNode(std::shared_ptr<Node> node) {
        m_nodes.push_back(node);

        m_next_id += node->GetNodeIDOffset();
    }

    void NodeGraph_UI::RenderGraph() {
        ImNodes::BeginNodeEditor();

        ManageContextMenu();
        if(ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImNodes::IsEditorHovered()) {
            ImGui::OpenPopup("addNode");
        }

        for(auto& node : m_nodes) {
            node->RenderNode();
            node->RenderLinks();
        }

        bool editorHovered = ImNodes::IsEditorHovered();
        ImNodes::EndNodeEditor();

        int linkID_start, linkID_end;
        if(ImNodes::IsLinkCreated(&linkID_start, &linkID_end)) {
            std::shared_ptr<Node> startNode;
            std::shared_ptr<Node> endNode;

            for(auto n : m_nodes) {
                if(n->ContainsPin(linkID_start)) {
                    startNode = n;
                } else if(n->ContainsPin(linkID_end)) {
                    endNode = n;
                }
            }

            if(endNode->ConnectPins(startNode, linkID_start, linkID_end, m_linkCount)) {
                m_linkCount++;
            }

            for(auto& n : m_nodes) {
                n->ValidateLinks();
            }
        }

        if(ImGui::IsKeyPressed(ImGuiKey_Delete, false) && editorHovered) {
            DeleteSelectedNodes();
            DeleteSelectedLinks();
        }
    }

    void NodeGraph_UI::AddNodeToCatalog(std::string cat, std::string name, std::function<std::shared_ptr<Node>(int)> factory) {
        bool catFound = false;
        NodeCatalogCat* category;

        for(auto& c : m_catalog) {
            if(c.catName == cat) {
                catFound = true;
                category = &c;
                break;
            }
        }

        if(!catFound) {
            NodeCatalogCat newCat{};
            newCat.catName = cat;
            newCat.items.push_back({name, factory});

            m_catalog.push_back(newCat);

            return;
        }

        category->items.push_back({name, factory});
    }

    void NodeGraph_UI::ManageContextMenu() {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 6.f));

        if(ImGui::BeginPopupContextItem("addNode")) {
            ImGui::Text("Add Node");
            ImGui::Separator();

            for(auto& c : m_catalog) {
                if(ImGui::BeginMenu(c.catName.c_str())) {
                    for(auto& item : c.items) {
                        if(ImGui::MenuItem(item.name.c_str())) {
                            int currentID = m_next_id;
                            AddNode(item.factory(m_next_id));

                            ImNodes::SetNodeScreenSpacePos(currentID, ImGui::GetMousePos());
                        }
                    }

                    ImGui::EndMenu();
                }
            }

            ImGui::EndPopup();
        }

        ImGui::PopStyleVar(2);
    }

    void NodeGraph_UI::RenderNodeProperties() {
        int selectedCount = ImNodes::NumSelectedNodes();

        int* selNodes = new int[selectedCount];
        ImNodes::GetSelectedNodes(selNodes);

        bool requestLinkValidation = false;

        for(auto& n : m_nodes) {
            for(int i = 0; i < selectedCount; i++) {
                if(n->GetNodeID() == selNodes[i]) {
                    requestLinkValidation |= n->RenderProperties();
                    break;
                }
            }
        }

        if(requestLinkValidation) {
            for(auto& n : m_nodes) {
                n->ValidateLinks();
            }
        }

        delete[] selNodes;
    }

    void NodeGraph_UI::DeleteSelectedNodes() {
        int selectedCount = ImNodes::NumSelectedNodes();

        int* selNodes = new int[selectedCount];
        ImNodes::GetSelectedNodes(selNodes);

        std::erase_if(m_nodes, [selNodes, selectedCount](std::shared_ptr<Node>& n) {
            for(int i = 0; i < selectedCount; i++) {
                if(selNodes[i] == n->GetNodeID() && selNodes[i] != 0) {
                    n.reset();
                    return true;
                }
            }

            return false;
        });

        for(auto& n : m_nodes) {
            n->ValidateLinks();
        }

        delete[] selNodes;
    }

    void NodeGraph_UI::DeleteSelectedLinks() {
        int selectedCount = ImNodes::NumSelectedLinks();

        int* selLinks = new int[selectedCount];
        ImNodes::GetSelectedLinks(selLinks);

        for(int i = 0; i < m_nodes.size(); i++) {
            for(int a = 0; a < selectedCount; a++) {
                if(m_nodes[i]->ContainsLink(selLinks[a])) {
                    m_nodes[i]->DeleteLink(selLinks[a]);
                }
            }
        }

        for(auto& n : m_nodes) {
            n->ValidateLinks();
        }
    }

    std::shared_ptr<Node> NodeGraph_UI::GetNodeByID(int id) {
        for(auto& n : m_nodes) {
            if(n->GetNodeID() == id) {
                return n;
            }
        }

        return {};
    }

    bool NodeGraph_UI::SaveGraphToFile(std::string filepath) {
        std::ofstream f(filepath, std::ios::binary);
        if(!f.is_open()) {
            return false;
        }

        uint32_t nodeCount = m_nodes.size();
        f.write((char*)&nodeCount, sizeof(uint32_t));

        for(auto& n : m_nodes) {
            n->Serialize(f);
        }

        f.close();

        return true;
    }

    bool NodeGraph_UI::LoadGraphFromFile(std::string filepath) {
        std::ifstream f(filepath, std::ios::binary);
        if(!f.is_open()) {
            return false;
        }

        m_nodes.clear();

        uint32_t nodeCount;
        f.read((char*)&nodeCount, sizeof(uint32_t));

        std::vector<PreLoadedNode> nodes;
        for(int i = 0; i < nodeCount; i++) {
            PreLoadedNode n;

            // Node Name
            uint32_t nameSize;
            f.read((char*)&nameSize, sizeof(uint32_t));

            char* nodeName = new char[nameSize + 1];
            f.read(nodeName, nameSize);
            nodeName[nameSize] = '\0';

            n.nodeName = std::move(nodeName);

            // Node ID
            f.read((char*)&n.id, sizeof(uint32_t));

            // Links
            uint32_t linkSize;
            f.read((char*)&linkSize, sizeof(uint32_t));
            for(int a = 0; a < linkSize; a++) {
                NodeConnection_Serializable l;
                f.read((char*)&l, sizeof(NodeConnection_Serializable));

                n.links.push_back(l);
            }

            // Node Pos
            f.read((char*)&n.pos, sizeof(ImVec2));

            // Properties
            uint32_t propsize;
            f.read((char*)&propsize, sizeof(uint32_t));

            n.properties.resize(propsize);
            f.read(n.properties.data(), propsize);

            nodes.push_back(n);
        }

        f.close();

        // Instanciating actual nodes
        for(auto& prenode : nodes) {
            if(prenode.id == 0) {
                std::shared_ptr<Node> newNode = std::make_shared<ShaderNode>(0);
                newNode->SetNodePos(prenode.pos);

                AddNode(newNode);

                continue;
            }

            for (auto &c: m_catalog) {
                for (auto &nt: c.items) {
                    if (nt.name == prenode.nodeName) {
                        std::shared_ptr<Node> newNode = nt.factory(prenode.id);
                        newNode->SetNodePos(prenode.pos);

                        AddNode(newNode);
                    }
                }
            }
        }

        for(auto& prenode : nodes) {
            // Resolve Links
            for (auto &l: prenode.links) {
                uint32_t otherID = l.otherNodeID;
                auto otherNodeIT = std::find_if(m_nodes.begin(), m_nodes.end(), [otherID](std::shared_ptr<Node>& n) {
                    return n->GetNodeID() == otherID;
                });

                if(otherNodeIT == m_nodes.end()) {
                    return false;
                }

                NodeConnection realLink{};
                realLink.linkID = l.linkID;
                realLink.src_pinID = l.src_pinID;
                realLink.dst_pinID = l.dst_pinID;
                realLink.OtherNode = std::weak_ptr<Node>(*otherNodeIT);

                prenode.realLinks.push_back(realLink);
            }

            std::shared_ptr<Node> realNode = GetNodeByID(prenode.id);
            realNode->LoadLinks(prenode.realLinks);
        }

        m_next_id = m_nodes.back()->GetNodeID() + m_nodes.back()->GetNodeIDOffset();

        return true;
    }

} // OmbrageUI