#ifndef EXPGE_GAMELEVEL_H
#define EXPGE_GAMELEVEL_H

#include <EXP_HotLoad.h>
#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_Level.h>
#include <EXP_Animator.h>

#include <EXP_Material.h>
#include <EXP_StaticMeshActor.h>
#include <EXP_SkelAnim.h>
#include <EXP_KeyboardCallback.h>

#include <EXP_WidgetBasic.h>

#include <RD_Mesh.h>
#include <RD_Skeleton.h>

#include <RD_Camera.h>
#include <RD_DirLight.h>

#include "TestActor.h"

class GameLevel : public EXP_Level {
public:
    GameLevel(EXP_Game* game);
    ~GameLevel();

    void OnStart() override;

    void PlayPause();
private:
    EXP_Game* m_game;

    std::shared_ptr<TestActor> m_tactor;

    std::shared_ptr<EXP_SkelAnim> m_anim;

    std::shared_ptr<EXP_KeyboardCallback> m_playpause;
    bool m_pause_state;
};

EXPGE_LEVEL_HEADER(GameLevel);

#endif //EXPGE_GAMELEVEL_H
