#include <iostream>
#include <thread>

#include <EXP_Game.h>
#include <EXP_MapLoader.h>
#include <EXP_HotLoad.h>

int main(int argc, char* argv[]) {
	EXP_Game* game = new EXP_Game("GameInfo.json");

	std::thread phys;
	std::thread snd;

	while (!game->GetRenderer()->WantToClose()) {
		game->RenderScene();
		game->ExecCallbacks();

		game->UpdatePhysics();
		game->UpdateSound();

		game->EndFrame();
	}
	
	delete game;

	return 0;
}