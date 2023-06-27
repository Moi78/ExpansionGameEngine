#include "OmbrageCamera.h"

OmbrageUI::OmbrageCamera::OmbrageCamera(EXP_Game *game) : EXP_Actor(vec3(-4.0f, 0.0f, 1.0f), vec3(), vec3(1.0f, 1.0f, 1.0f)) {
    m_cam_comp = std::make_shared<EXP_Camera>(game, vec3(), vec3(), 60.0f, 0.1f, 100.0f);
    LinkComponent(m_cam_comp);
    game->GetRenderer()->SetCurrentCamera(m_cam_comp);
}

OmbrageUI::OmbrageCamera::~OmbrageCamera() {

}

void OmbrageUI::OmbrageCamera::OnStart() {

}

void OmbrageUI::OmbrageCamera::OnTick() {

}
