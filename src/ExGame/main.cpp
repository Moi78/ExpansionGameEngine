#include <iostream>

#include <EXP_Game.h>
#include <EXP_MapLoader.h>

#include <EXP_HotLoad.h>

#include <BD_MatRW.h>

int main(int argc, char* argv[]) {
	EXP_Game* game = new EXP_Game("GameInfo.json");

	while (!game->GetRenderer()->WantToClose()) {
		game->MainLoop();
	}
	
	delete game;

	return 0;
}