#ifndef EXPGE_NODEGRAPH_UI_H
#define EXPGE_NODEGRAPH_UI_H

#include <memory>

#include <imnodes.h>

#include "Node.h"

namespace OmbrageUI {

    class NodeGraph_UI {
    public:
        NodeGraph_UI();
        ~NodeGraph_UI();

        void RenderGraph();

        void SetRootNode(std::shared_ptr<Node> rootNode);

    private:
        std::shared_ptr<Node> m_root_node;
    };

} // OmbrageUI

#endif //EXPGE_NODEGRAPH_UI_H
