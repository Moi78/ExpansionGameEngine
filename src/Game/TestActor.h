#ifndef EXPGE_TESTACTOR_H
#define EXPGE_TESTACTOR_H

#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <EXP_Camera.h>

#include <RD_Camera.h>

class TestActor : public EXP_Actor {
public:
    TestActor(EXP_Game* game);
    ~TestActor();

    void OnStart() override;
    void OnTick() override;

private:
    std::shared_ptr<EXP_Camera> m_cam;
};


#endif //EXPGE_TESTACTOR_H
