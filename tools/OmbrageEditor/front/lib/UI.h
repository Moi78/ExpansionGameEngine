#ifndef EXPGE_UI_H
#define EXPGE_UI_H

#include "imgui_internal.h"

#include <EXP_Game.h>
#include <EXP_ImGuiClass.h>
#include <EXP_Level.h>
#include <EXP_StaticMeshActor.h>

#include <RD_Texture.h>

#include <SpirvCompiler.h>
#include <FunctionGraph.h>

#include "NodeGraph_UI.h"

#include "ShaderNode.h"
#include "MathNodes.h"
#include "SplitNode.h"
#include "CombineNode.h"
#include "ShaderInNode.h"
#include "TextureNode.h"
#include "BumpNode.h"

#include "third_party/portable-file-dialogs.h"

#define IMVEC2_DIFF(a, b) ((a.x != b.x) || (a.y != b.y))

enum CompileState {
    DEPS_LOAD,
    COMPILING_SHADER,
    DONE
};

namespace OmbrageUI {
    class UI : public EXP_ImGuiClass {
    public:
        UI(EXP_Game *game);
        ~UI();

        void SetupViewport();
        void RenderImGui() override;

    private:
        void DockSpaceHandling();
        void MainMenuBar();

        bool CompileLoadDeps();
        bool CompileShader();

        void New();
        void Save();
        void SaveAs();
        void Open();
        void Export();

        EXP_Game* m_game;

        std::unique_ptr<NodeGraph_UI> m_node_editor;

        ImVec2 m_oldSize;
        ImVec2 m_oldPos;

        std::shared_ptr<SpirvCompiler> m_compiler;

        std::vector<uint32_t> m_lastCompiledData;
        std::unordered_map<std::string, int> m_texmap;

        CompileState m_cstate;

        std::string m_projContentRoot;

        bool m_isFileLoaded;
        bool m_isFileSaved;
        std::string m_savepath;
    };
}


#endif //EXPGE_UI_H
