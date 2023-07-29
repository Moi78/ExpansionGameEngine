#ifndef EXPGE_NODE_H
#define EXPGE_NODE_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "imnodes.h"

#include "SpirvCompiler.h"
#include "FunctionGraph.h"

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

    std::string pinName;

    uint32_t pinID;
};

ImVec4 MapTypeToColor(NodePinTypes t);

// Returns if weight(a) >= weight(b)
bool TypeWeightCompare(NodePinTypes a, NodePinTypes b);

// Returns if the two types can be used together in a math op
bool TypeCompatibility(NodePinTypes a, NodePinTypes b);

std::string GetTypeSuffix(NodePinTypes t);

struct Node;
struct NodeConnection {
    std::shared_ptr<Node> OtherNode;

    uint32_t dst_pinID;
    uint32_t src_pinID;

    uint32_t linkID;
};

struct Constant {
    std::shared_ptr<SpirvConstant> ctant;

    virtual void MakeCtant() {};
};

class Node {
public:
    Node(uint32_t id);
    virtual ~Node() {};

    void RenderNode();
    void RenderLinks();

    uint32_t GetNodeID() { return m_id; }

    virtual bool RenderProperties() { return false; };

    bool ContainsPin(uint32_t globID);
    virtual bool ConnectPins(std::shared_ptr<Node> other, uint32_t srcID, uint32_t pID, uint32_t linkID);
    virtual void ValidateLinks();

    NodePinTypes GetPinType(uint32_t globID);
    NodePinMode GetPinMode(uint32_t globID);

    uint32_t GetNodeSize() { return m_io.size(); };
    virtual uint32_t GetNodeIDOffset() { return m_io.size(); } // Separating to allow dynamics nodes to have enough IDs
                                                               // allocated and return a higher offset than the size of
                                                               // its IO which can change depending on the input

    std::shared_ptr<Node> GetParent(int pinID);

    virtual std::string GetNodeFunctionName() = 0;
    bool isConst() { return m_isConst; };

    virtual std::shared_ptr<FuncGraphElem> EvalFrom(int locID, std::shared_ptr<SpirvCompiler> compiler);

    virtual std::shared_ptr<SpOpFunCall> MakeFunctionCall(std::shared_ptr<SpirvCompiler> compiler, std::string funcName, std::vector<std::shared_ptr<SpirvDataWrapperBase>> args, int outID);

protected:
    std::shared_ptr<SpirvDataWrapperBase> GetDefaultValue(std::shared_ptr<SpirvCompiler> compiler, NodePinTypes type);
    bool CheckConnectionValidity(NodePinTypes atype, NodePinTypes btype, int destPin);

    uint32_t m_id;
    std::string m_nodeName;

    std::vector<NodePinDesc> m_io;
    std::vector<NodeConnection> m_connections;

    float m_color[3];
    bool m_isConst;
};

#endif //EXPGE_NODE_H
