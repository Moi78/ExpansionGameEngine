#ifndef EXPGE_NODE_H
#define EXPGE_NODE_H

#include <unordered_map>
#include <memory>

class Node {
public:
    Node() = default;
    virtual ~Node() {};

    void RenderHierachy() {
        RenderNode();

        for(auto& c : m_childs) {
            c.second->RenderHierachy();
        }
    }

    virtual void RenderNode() = 0;
    virtual void ConnectTo(int sourceId, int dstId) = 0;

private:
    std::unordered_map<int, std::shared_ptr<Node>> m_childs;
};


#endif //EXPGE_NODE_H
