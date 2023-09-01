#ifndef EXPGE_BUMPNODE_H
#define EXPGE_BUMPNODE_H

#include "Node.h"

namespace OmbrageNodes {
    class BumpNode : public Node {
    public:
        BumpNode(int id);
        ~BumpNode() = default;

        std::string GetNodeFunctionName() override { return "texture/bump"; };
    };
}

#endif //EXPGE_BUMPNODE_H
