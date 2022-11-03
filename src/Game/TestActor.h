#ifndef EXPGE_TESTACTOR_H
#define EXPGE_TESTACTOR_H

#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <EXP_Camera.h>
#include <EXP_KeyboardCallback.h>
#include <EXP_PointLight.h>
#include <EXP_StaticMesh.h>

class TestActor : public EXP_Actor {
public:
    TestActor(EXP_Game* game);
    ~TestActor();

    void OnStart() override;
    void OnTick() override;

    void Forward();

private:
    std::shared_ptr<EXP_InputHandler> m_inhdl;

    std::shared_ptr<EXP_Camera> m_cam;
    std::shared_ptr<EXP_PointLight> m_plight;

    std::shared_ptr<EXP_KeyboardCallback> m_forward;
};


#endif //EXPGE_TESTACTOR_H
