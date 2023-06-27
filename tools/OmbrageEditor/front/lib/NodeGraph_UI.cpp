#include "NodeGraph_UI.h"

namespace OmbrageUI {
    NodeGraph_UI::NodeGraph_UI() {

    }

    NodeGraph_UI::~NodeGraph_UI() {

    }

    void NodeGraph_UI::SetRootNode(std::shared_ptr<Node> rootNode) {
        m_root_node = rootNode;
    }

    void NodeGraph_UI::RenderGraph() {
        ImNodes::BeginNodeEditor();

        if(m_root_node.use_count()) {
            m_root_node->RenderNode();
        }

        ImNodes::EndNodeEditor();
    }
} // OmbrageUI