#include "OmbrageMainLevel.h"

EXPGE_LEVEL_CODE(OmbrageMainLevel);

OmbrageMainLevel::OmbrageMainLevel(EXP_Game* game) : EXP_Level(game) {
    m_ui = std::make_shared<OmbrageUI::UI>(game);
    game->AddImGuiClass(m_ui);

    m_cam = std::make_shared<OmbrageUI::OmbrageCamera>(game);
    RegisterActor(m_cam);
}

OmbrageMainLevel::~OmbrageMainLevel() {

}

void OmbrageMainLevel::OnStart() {
    std::cout << "OmbrageUI Level Started" << std::endl;

    m_ui->SetupViewport();
}
