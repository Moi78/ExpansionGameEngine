#ifndef EXPGE_MATHNODES_H
#define EXPGE_MATHNODES_H

#include "Node.h"
#include <iostream>

namespace OmbrageNodes {
    class ConstFloatNode : public Node {
    public:
        ConstFloatNode(uint32_t id);
    };

    class ConstVec4Node : public Node {
    public:
        ConstVec4Node(uint32_t id);
    };

    class ConstVec3Node : public Node {
    public:
        ConstVec3Node(uint32_t id);
    };

    class ConstVec2Node : public Node {
    public:
        ConstVec2Node(uint32_t id);
    };

    class MathNode : public Node {
    public:
        MathNode(uint32_t id) : Node(id) {}

        bool LinkPostCheck(std::shared_ptr<Node> other, uint32_t src, uint32_t dest);
        bool ConnectPins(std::shared_ptr<Node> other, uint32_t srcID, uint32_t pID, uint32_t linkID) override;

        void ValidateLinks() override;
    };

    class AddNode : public MathNode {
    public:
        AddNode(uint32_t id);
    };

    class MultNode : public MathNode {
    public:
        MultNode(uint32_t id);
    };
}


#endif //EXPGE_MATHNODES_H
