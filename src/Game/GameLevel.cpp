#include "GameLevel.h"

EXPGE_LEVEL_CODE(GameLevel);

GameLevel::GameLevel(EXP_Game* game) : EXP_Level(game) {
    std::cout << "INSTANCIATED" << std::endl;

    auto mat = std::make_shared<EXP_Material>(game);
    mat->LoadMaterial("/mat/mat_test.json");

    auto mesh = std::make_shared<EXP_StaticMeshActor>(game, "/mdl/monkey.msh", mat, vec3(), vec3(0.0f, 10.0f, -45.0f), vec3(1.0f, 1.0f, 1.0f));
    RegisterActor(mesh);

    m_tactor = std::make_shared<TestActor>(game);
    RegisterActor(m_tactor);

    auto dlight = std::make_shared<RD_DirLight>(vec3(0.5f, 0.5, -1.0f), 1.0f, vec3(1.0f, 1.0f, 1.0f));
    game->GetRenderer()->RegisterDirLight(dlight);
}

GameLevel::~GameLevel() {
    std::cout << "DESTROYED" << std::endl;
}