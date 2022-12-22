#include "TestActor.h"

TestActor::TestActor(EXP_Game* game) : EXP_Actor(vec3(-2.0f, -2.0f, 0.0f), vec3(), vec3(1.0f, 1.0f, 1.0f)) {
    m_inhdl = game->GetInputHandler();

    m_cam = std::make_shared<EXP_Camera>(game, vec3(), vec3(), 60.0f, 0.01f, 100.0f);
    LinkComponent(m_cam);
    game->GetRenderer()->SetCurrentCamera(m_cam);

    m_plight = std::make_shared<EXP_PointLight>(game, vec3(2.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), 5.0f, 0.0f);
    LinkComponent(m_plight);
    game->GetRenderer()->RegisterPointLight(m_plight);

    m_forward = std::make_shared<EXP_KeyboardCallback>('Z', CL_VDFUNCPTR(TestActor::Forward));
    m_inhdl->RegisterKeyboardCallback(m_forward);
}

TestActor::~TestActor() {

}

void TestActor::OnStart() {

}

void TestActor::OnTick() {
    m_cam->AddYaw(m_inhdl->GetCursorPosX() * 0.1f);
    m_cam->AddPitch(m_inhdl->GetCursorPosY() * -0.1f);

    RotateActor(vec3(0.0f, 0.0f, m_inhdl->GetCursorPosX() * 0.1f));

    //m_plight->SetComponentPos(vec3());
}

void TestActor::Forward() {
    TranslateActor(m_cam->GetForwardVector() * 0.05f);
}