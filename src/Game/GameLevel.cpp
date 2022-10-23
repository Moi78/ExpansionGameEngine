#include "GameLevel.h"

EXPGE_LEVEL_CODE(GameLevel);

GameLevel::GameLevel(EXP_Game* game) : EXP_Level(game) {
    std::cout << "INSTANCIATED" << std::endl;

    auto mat = std::make_shared<EXP_Material>(game);
    mat->LoadMaterial("/mat/mat_test.json");

    auto mesh = std::make_shared<EXP_StaticMeshActor>(game, "/mdl/monkey.msh", mat, vec3(), vec3(0.0f, 0.0f, -45.0f), vec3(1.0f, 1.0f, 1.0f));
    RegisterActor(mesh);

    auto cam = std::make_shared<RD_Camera>(game->GetRenderer()->GetAPI(), 30.0f, vec3(-5.0f, -5.0f, 0.0f), vec3(), 0.001f, 1000.0f);
    game->GetRenderer()->SetCurrentCamera(cam);

    auto dlight = std::make_shared<RD_DirLight>(vec3(0.5f, 0.5, -1.0f), 1.0f, vec3(1.0f, 1.0f, 1.0f));
    game->GetRenderer()->RegisterDirLight(dlight);
}

GameLevel::~GameLevel() {
    std::cout << "DESTROYED" << std::endl;
}