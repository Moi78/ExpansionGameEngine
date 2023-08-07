#ifndef EXPGE_TEXTURENODE_H
#define EXPGE_TEXTURENODE_H

#include "Node.h"
#include "imgui_stdlib.h"

#include <fmt/core.h>

namespace OmbrageNodes {

    class TextureNode : public Node {
    public:
        TextureNode(int id);
        ~TextureNode() = default;

        std::string GetNodeFunctionName() override;
        std::shared_ptr<SpOpFunCall> MakeFunctionCall(
                std::shared_ptr<SpirvCompiler> compiler,
                std::string funcName,
                std::vector<std::shared_ptr<SpirvDataWrapperBase>> args,
                int outID) override;

        std::string GetTexturePath() override;

        bool RenderProperties() override;
        std::vector<char> SerializeProperties() override;
        void LoadProperties(std::vector<char> data) override;

    private:
        std::string m_texPath;
    };

} // OmbrageNodes

#endif //EXPGE_TEXTURENODE_H
