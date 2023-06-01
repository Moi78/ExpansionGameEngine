#include "OmbrageMainLevel.h"

EXPGE_LEVEL_CODE(OmbrageMainLevel);

OmbrageMainLevel::OmbrageMainLevel(EXP_Game* game) : EXP_Level(game) {
    m_ui = std::make_shared<Ombrage::UI>(game);
    m_ui->ContructUI();

    m_cam = std::make_shared<Ombrage::OmbrageCamera>(game);
    RegisterActor(m_cam);
}

OmbrageMainLevel::~OmbrageMainLevel() {

}

void OmbrageMainLevel::OnStart() {
    std::cout << "Ombrage Level Started" << std::endl;
}
