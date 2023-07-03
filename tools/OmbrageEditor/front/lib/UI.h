#ifndef EXPGE_UI_H
#define EXPGE_UI_H

#include <EXP_Game.h>
#include <EXP_ImGuiClass.h>

#include "NodeGraph_UI.h"

#include "ShaderNode.h"
#include "MathNodes.h"

#define IMVEC2_DIFF(a, b) ((a.x != b.x) || (a.y != b.y))

namespace OmbrageUI {
    class UI : public EXP_ImGuiClass {
    public:
        UI(EXP_Game *game);
        ~UI();

        void SetupViewport();
        void RenderImGui() override;

    private:
        void DockSpaceHandling();

        EXP_Game* m_game;

        std::unique_ptr<NodeGraph_UI> m_node_editor;

        ImVec2 m_oldSize;
        ImVec2 m_oldPos;
    };
}


#endif //EXPGE_UI_H
