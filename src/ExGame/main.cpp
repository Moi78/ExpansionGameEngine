#include <iostream>
#include <memory>

#include <RaindropRenderer.h>
#include <RD_API.h>

#include <EXP_Game.h>
#include <EXP_Conf.h>

//Enabling Nvidia Optimus
extern "C" { uint32_t NvOptimusEnablement = 0x00000001; }

int main(int argc, char* argv[]) {
	std::cout << "EXPANSION GAME ENGINE REWRITE" << std::endl;

    EXP_GameInfo gameinfo{};
    gameinfo.GameLib = "./libGame.so";
    gameinfo.RootEngineContentDir = "./Engine/";
    gameinfo.RootGameContentDir = "./Content/";
    gameinfo.WindowHeight = 1270;
    gameinfo.WindowWidth = 720;
    gameinfo.StartLevel = "/levels/level1.json";

	std::shared_ptr<RD_API> api = std::make_shared<RD_API_Vk>();
    std::shared_ptr<RD_RenderingPipeline> rpline = std::make_shared<RD_RenderingPipeline_PBR>(api);

	std::shared_ptr<RaindropRenderer> rndr = std::make_shared<RaindropRenderer>(std::shared_ptr<RD_API>(api), std::shared_ptr<RD_RenderingPipeline>(rpline), 1270, 720, "ExGame");

    std::unique_ptr<EXP_Game> game = std::make_unique<EXP_Game>(rndr, gameinfo);

    if(!game->InitEngine()) {
        std::cerr << "ERROR: Failed to init engine. :(" << std::endl;
        return -1;
    }

    game->RunGame();

	return 0;
}
