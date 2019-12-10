#include <iostream>

#include <EXP_Game.h>
#include <EXP_StaticMesh.h>

#include <vec3.h>
#include <BD_StructMan.h>

int main(int argc, char* argv[]) {
	BD_GameInfo gi = {};
	gi.RootEngineContentFolder = "Engine";
	gi.RootGameContentFolder = "Content/Character";

	EXP_Game* game = new EXP_Game({ 1280, 720 }, gi, vec3f(0.0f, 0.0f, 0.0f), "PresentDrop");
	game->GetRenderer()->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	game->GetRenderer()->SetAmbientStrength(0.3f);

	game->GetRenderer()->DisableFeature(RendererFeature::Lighting);

	BD_MatDef mat = {};
	mat.Color = vec3f(1.0f, 0.0f, 0.0f);
	mat.SpecularColor = vec3f(1.0f, 1.0f, 1.0f);
	mat.SpecularExp = 32.0f;

	EXP_StaticMesh* testMesh = new EXP_StaticMesh(game, "/cube.msh", mat, vec3f(), vec3f(), vec3f(1.0f, 1.0f, 1.0f));

	game->LaunchMainLoop();

	return 0;
}