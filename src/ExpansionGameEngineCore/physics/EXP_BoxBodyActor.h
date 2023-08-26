#ifndef EXPGE_EXP_BOXBODYACTOR_H
#define EXPGE_EXP_BOXBODYACTOR_H

#include "EXP_Actor.h"
#include "EXP_PhysicsHandler.h"
#include "EXP_BoxBody.h"

class EXP_BoxBodyActor : public EXP_Actor {
public:
    EXP_BoxBodyActor(std::shared_ptr<EXP_PhysicsHandler> handler, vec3 pos, vec3 rot, vec3 scale);
    ~EXP_BoxBodyActor() = default;

    void OnStart() override;
    void OnTick() override;

private:
    std::unique_ptr<EXP_BoxBody> m_body;
};


#endif //EXPGE_EXP_BOXBODYACTOR_H
