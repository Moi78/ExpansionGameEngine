#include <iostream>

#include <EXP_Game.h>

#include <vec3.h>
#include <BD_StructMan.h>

int main(int argc, char* argv[]) {
	BD_GameInfo gi = {};
	gi.RootEngineContentFolder = "./Engine";
	gi.RootGameContentFolder = "./Content";

	EXP_Game* game = new EXP_Game({ 1280, 720 }, gi, vec3f(0.0f, 0.0f, 0.0f), "PresentDrop");

	game->LaunchMainLoop();

	return 0;
}