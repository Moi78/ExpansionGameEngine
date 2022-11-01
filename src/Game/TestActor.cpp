#include "TestActor.h"

TestActor::TestActor(EXP_Game* game) : EXP_Actor(vec3(-2.0f, -2.0f, 0.0f), vec3(), vec3(1.0f, 1.0f, 1.0f)) {
    m_cam = std::make_shared<EXP_Camera>(game, vec3(), vec3(), 60.0f, 0.001f, 1000.0f);
    LinkComponent(m_cam);
    game->GetRenderer()->SetCurrentCamera(m_cam);

    m_cpt = 0.0f;
}

TestActor::~TestActor() {

}

void TestActor::OnStart() {

}

void TestActor::OnTick() {
    m_cam->AddRoll(sin(m_cpt));
    m_cam->AddYaw(1.0f);

    m_cpt += 0.1f;
}