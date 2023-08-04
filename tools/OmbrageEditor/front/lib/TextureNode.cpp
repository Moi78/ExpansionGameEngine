#include "TextureNode.h"

namespace OmbrageNodes {
    TextureNode::TextureNode(int id) : Node(id) {
        m_io = {
                {NodePinMode::INPUT, NodePinTypes::VEC2, "UV", 0},
                {NodePinMode::OUTPUT, NodePinTypes::VEC4, "Sampled", 1}
        };

        m_nodeName = "Sample Texture";

        m_holdTexture = true;
    }

    std::string TextureNode::GetNodeFunctionName() {
        return "texture/sampleTex";
    }

    std::string TextureNode::GetTexturePath() {
        return m_texPath;
    }

    bool TextureNode::RenderProperties() {
        if(ImGui::CollapsingHeader(fmt::format("Sample Texture##{}", m_id).c_str())) {
            ImGui::InputText("Texture Path", &m_texPath);
        }

        return false;
    }

    std::shared_ptr<SpOpFunCall> TextureNode::MakeFunctionCall(
            std::shared_ptr<SpirvCompiler> compiler, std::string funcName,
            std::vector<std::shared_ptr<SpirvDataWrapperBase>> args, int outID
    ) {
        auto function = compiler->GetFunction(funcName);

        args.push_back(compiler->GetTextureW(m_texPath));

        auto fcall = std::make_shared<SpOpFunCall>(function, args, true);
        compiler->AddFunCallToEntry(fcall);

        return fcall;
    }

} // OmbrageNodes
