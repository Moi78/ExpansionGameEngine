#include "GameLevel.h"

EXPGE_LEVEL_CODE(GameLevel);

GameLevel::GameLevel(EXP_Game* game) : EXP_Level(game) {
    std::cout << "INSTANCIATED" << std::endl;

    game->GetInputHandler()->SetCursorVisibility(false);

    m_tactor = std::make_shared<TestActor>(game);
    RegisterActor(m_tactor);

    auto ft = game->GetGuiManager()->ConstructFont(game, "/fonts/simplysans.ttf", false);
    ft->LoadFont();

    auto text = std::make_shared<EXP_GuiTextStatic>(game, RD_Rect{200, 100, 0, 0}, ft);
    text->ConstructText("Hellog World\nYYYYY\nNew freaking line of text\nMultiline is kewl", 25);

    auto btn = std::make_shared<EXP_GuiButton>(game, RD_Rect{500, 500, 300, 100}, "/tex/grad.png", "/tex/test.jpg", "/tex/grad.png");
    auto btnText = std::make_shared<EXP_GuiTextStatic>(game, RD_Rect{0, 0, 0, 0}, ft, btn);
    btnText->ConstructText("This is a button", 30);

    game->GetGuiManager()->AddWidget(text);
    game->GetGuiManager()->AddWidget(btn);
    game->GetGuiManager()->AddWidget(btnText);

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