#ifndef EXPGE_NODE_H
#define EXPGE_NODE_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "imnodes.h"

enum NodePinTypes {
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    OMNI
};

enum NodePinMode {
    INPUT,
    OUTPUT
};

struct NodePinDesc {
    NodePinMode mode;
    NodePinTypes type;

    char pinName[50];

    uint32_t pinID;
};

ImVec4 MapTypeToColor(NodePinTypes t);

// Returns if weight(a) >= weight(b)
bool TypeWeightCompare(NodePinTypes a, NodePinTypes b);

// Returns if the two types can be used together in a math op
bool TypeCompatibility(NodePinTypes a, NodePinTypes b);

struct NodeConnection;

class Node {
public:
    Node(uint32_t id);
    virtual ~Node() {};

    void RenderNode();
    void RenderLinks();

    bool ContainsPin(uint32_t globID);
    virtual bool ConnectPins(std::shared_ptr<Node> other, uint32_t srcID, uint32_t pID, uint32_t linkID);
    virtual void ValidateLinks();

    NodePinTypes GetPinType(uint32_t globID);

    uint32_t GetNodeSize() { return m_io.size(); };

protected:
    bool CheckConnectionValidity(NodePinTypes atype, NodePinTypes btype, int destPin);

    uint32_t m_id;
    std::string m_nodeName;

    std::vector<NodePinDesc> m_io;
    std::vector<NodeConnection> m_connections;
};

struct NodeConnection {
    std::shared_ptr<Node> OtherNode;

    uint32_t dst_pinID;
    uint32_t src_pinID;

    uint32_t linkID;
};

#endif //EXPGE_NODE_H
