#include "GameLevel.h"

EXPGE_LEVEL_CODE(GameLevel);

GameLevel::GameLevel(EXP_Game* game) : EXP_Level(game) {
    std::cout << "INSTANCIATED" << std::endl;

    game->GetInputHandler()->SetCursorVisibility(false);

    m_tactor = std::make_shared<TestActor>(game);
    RegisterActor(m_tactor);

    m_bbactor = std::make_shared<EXP_BoxBodyActor>(game->GetPhysicsHandler(), vec3(0, 0, -1.5), vec3(0, 0, 0), vec3(10, 10, 0.5));
    RegisterActor(m_bbactor);

    m_bounce = std::make_shared<EXP_BoxBodyActor>(game->GetPhysicsHandler(), vec3(0.5f, 0.5f, 5.0f), vec3(), vec3(0.5f, 0.5f, 0.5f));
    RegisterActor(m_bounce);

    m_cube = std::make_shared<PhysicsCube>(game, vec3(1.0f, 1.0f, 10.0f), vec3(), vec3(1.5f, 2.5f, 0.5f));
    RegisterActor(m_cube);

    m_playpause = std::make_shared<EXP_KeyboardCallback>(GLFW_KEY_P, CL_VDFUNCPTR(GameLevel::PlayPause), false);

    m_game = game;
    m_pause_state = false;
}

GameLevel::~GameLevel() {
    std::cout << "DESTROYED" << std::endl;
}

void GameLevel::OnStart() {
    m_game->GetInputHandler()->RegisterKeyboardCallback(m_playpause);

    auto skelMeshActor = GetCastedActorByName<EXP_SkeletalMeshActor>("skel");
    auto skelMesh = skelMeshActor->GetMeshComponent();

    auto skelMeshActor2 = GetCastedActorByName<EXP_SkeletalMeshActor>("skel2");
    auto skelMesh2 = skelMeshActor2->GetMeshComponent();

    m_anim = std::make_shared<EXP_SkelAnim>(m_game);
    if(m_anim->LoadAnimation("/mdl/robot_anim.anim")) {
        m_game->GetAnimator()->PlayAnimation(m_anim, skelMesh, "skelAnim1", true);
        m_game->GetAnimator()->PlayAnimation(m_anim, skelMesh2, "skelAnim2", true);
    }
}

void GameLevel::PlayPause() {
    std::cout << "One time press" << std::endl;
    if(m_pause_state) {
        m_game->GetAnimator()->ResumeAnimation("skelAnim1");
        m_game->GetAnimator()->PauseAnimation("skelAnim2");
    } else {
       m_game->GetAnimator()->PauseAnimation("skelAnim1");
       m_game->GetAnimator()->ResumeAnimation("skelAnim2");
    }

    m_pause_state = !m_pause_state;
}