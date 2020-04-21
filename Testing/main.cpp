#include <iostream>

#include <EXP_Game.h>
#include <EXP_Camera.h>
#include <EXP_MapLoader.h>
#include <EXP_DirLight.h>
#include <EXP_PointLight.h>

#include "Donut.h"
#include "MainCharacter.h"

#include <vec3.h>


int main(int argc, char* argv[]) {
	BD_GameInfo gi = {};
	gi.RootEngineContentFolder = "Engine";
	gi.RootGameContentFolder = "Content";
	gi.GameName = "Demo Scene";

	EXP_Game* game = new EXP_Game({ 1280, 720 }, gi, vec3f(0.0f, 0.41f, 0.54f));
	game->GetRenderer()->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	game->GetRenderer()->SetAmbientStrength(0.1f);

	EXP_MapLoader* mload = new EXP_MapLoader(game, game->GetFilePathByRef("/Maps/map1.json"));
	mload->LoadMap();

	//EXP_DirLight* dlight = new EXP_DirLight(game, vec3f(1.0f, 0.0f, -0.5f), vec3f(1.0f, 1.0f, 1.0f), 1.0f);

	EXP_Camera* cam = new EXP_Camera(game, vec3f(0.0, -6.0, 2.0f), vec3f(), vec3f(), vec3f(90.0f), 60);
	cam->Use();

	while (!game->GetRenderer()->WantToClose()) {
		game->MainLoop();
	}

	return 0;
}