#ifndef EXPGE_NODEGRAPH_UI_H
#define EXPGE_NODEGRAPH_UI_H

#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <string>

#include <imnodes.h>

#include "Node.h"
#include "MathNodes.h"

namespace OmbrageUI {

    struct NodeCatalogItem {
        std::string name;
        std::function<std::shared_ptr<Node>(int)> factory;
    };

    struct NodeCatalogCat {
        std::string catName;
        std::vector<NodeCatalogItem> items;
    };

    struct PreLoadedNode {
        std::string nodeName;
        uint32_t id;

        std::vector<NodeConnection_Serializable> links;
        std::vector<NodeConnection> realLinks;
        ImVec2 pos;

        std::vector<char> properties;
    };

    class NodeGraph_UI {
    public:
        NodeGraph_UI();
        ~NodeGraph_UI();

        void RenderGraph();
        void RenderNodeProperties();

        void DeleteSelectedNodes();
        void DeleteSelectedLinks();

        void AddNode(std::shared_ptr<Node> Node);

        void AddNodeToCatalog(std::string cat, std::string name, std::function<std::shared_ptr<Node>(int)> factory);

        std::vector<std::shared_ptr<Node>>& GetNodes() { return m_nodes; }
        std::shared_ptr<Node> GetNodeByID(int id);

        bool SaveGraphToFile(std::string filepath);
        bool LoadGraphFromFile(std::string filepath);

    private:
        void ValidateAllLinks();
        void ManageContextMenu();

        std::vector<std::shared_ptr<Node>> m_nodes;
        std::vector<NodeCatalogCat> m_catalog;

        int m_linkCount;
        int m_next_id;
    };

} // OmbrageUI

#endif //EXPGE_NODEGRAPH_UI_H
