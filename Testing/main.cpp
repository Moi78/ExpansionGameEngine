#include <iostream>

#include <EXP_Game.h>
#include <EXP_Camera.h>
#include <EXP_MapLoader.h>
#include <EXP_DirLight.h>
#include <EXP_PointLight.h>

#include <EXP_HotLoad.h>

#include "Map1.h"

#include <vec3.h>

int main(int argc, char* argv[]) {
	EXP_Game* game = new EXP_Game("GameInfo.json");

	while (!game->GetRenderer()->WantToClose()) {
		game->MainLoop();
	}

	return 0;
}