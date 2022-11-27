#include "GameLevel.h"

EXPGE_LEVEL_CODE(GameLevel);

GameLevel::GameLevel(EXP_Game* game) : EXP_Level(game) {
    std::cout << "INSTANCIATED" << std::endl;

    game->GetInputHandler()->SetCursorVisibility(false);

    m_tactor = std::make_shared<TestActor>(game);
    RegisterActor(m_tactor);
}

GameLevel::~GameLevel() {
    std::cout << "DESTROYED" << std::endl;
}