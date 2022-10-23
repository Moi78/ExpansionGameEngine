#include "GameLevel.h"

EXPGE_LEVEL_CODE(GameLevel);

GameLevel::GameLevel(EXP_Game* game) : EXP_Level(game) {
    std::cout << "INSTANCIATED" << std::endl;
}

GameLevel::~GameLevel() {
    std::cout << "DESTROYED" << std::endl;
}