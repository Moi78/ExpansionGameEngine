#include <iostream>

#include <EXP_Game.h>

#include <BD_MatRW.h>

int main(int argc, char* argv[]) {
	EXP_Game* game = new EXP_Game("GameInfo.json");

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