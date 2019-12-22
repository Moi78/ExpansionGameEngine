#include <iostream>

#include <EXP_Game.h>
#include <EXP_StaticMesh.h>
#include <EXP_Camera.h>
#include <EXP_PointLight.h>

#include <vec3.h>
#include <BD_StructMan.h>

int main(int argc, char* argv[]) {
	BD_GameInfo gi = {};
	gi.RootEngineContentFolder = "Engine";
	gi.RootGameContentFolder = "Content";

	EXP_Game* game = new EXP_Game({ 1280, 720 }, gi, vec3f(0.0f, 0.0f, 0.0f), "PresentDrop");
	game->GetRenderer()->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	game->GetRenderer()->SetAmbientStrength(0.2f);
	
	BD_MatDef mat_cadeauBase = {};
	mat_cadeauBase.Color = vec3f(1.0f, 0.0f, 0.0f);
	mat_cadeauBase.SpecularColor = vec3f(1.0f, 1.0f, 1.0f);
	mat_cadeauBase.SpecularExp = 8.0f;

	BD_MatDef mat_cadeauRuban = {};
	mat_cadeauRuban.Color = vec3f(1.0f, 1.0f, 0.0f);
	mat_cadeauRuban.SpecularColor = vec3f(0.5f, 0.0f, 0.5f);
	mat_cadeauRuban.SpecularExp = 32.0f;

	//EXP_StaticMesh* cadeauBase = new EXP_StaticMesh(game, "/Cadeau/CadeauBase.msh", mat_cadeauBase, vec3f(), vec3f(0.0f, 0.0f, 0.0f), vec3f(1.0f, 1.0f, 1.0f));
	EXP_StaticMesh* cadeauRuban = new EXP_StaticMesh(game, "/cylinder.msh", mat_cadeauRuban, vec3f(), vec3f(), vec3f(0.5f, 0.5f, 0.5f));

	EXP_PointLight* testLight = new EXP_PointLight(game, vec3f(-1.0f, 0.0f, 3.5f), vec3f(), vec3f(), vec3f(1.0f, 1.0f, 1.0f), 15.0f);

	EXP_Camera* cam = new EXP_Camera(game, vec3f(-4.0f, 0.0f, 2.0f), vec3f(), vec3f(), vec3f(), 60.0f);
	cam->UseCamera();

	game->LaunchMainLoop();

	return 0;
}