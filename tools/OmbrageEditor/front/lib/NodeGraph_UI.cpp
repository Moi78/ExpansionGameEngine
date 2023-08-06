#include "NodeGraph_UI.h"

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

} // OmbrageUI