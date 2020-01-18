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

	EXP_Game* game = new EXP_Game({ 1280, 720 }, gi, vec3f(0.0f, 0.41f, 0.54f), "Demo Scene");
	game->GetRenderer()->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	game->GetRenderer()->SetAmbientStrength(0.2f);
	
	BD_MatDef mat_tonneau = {};
	mat_tonneau.Color = vec3f(0.4f, 0.2f, 0.11f);
	mat_tonneau.SpecularColor = vec3f(1.0f, 1.0f, 1.0f);
	mat_tonneau.SpecularExp = 8.0f;

	BD_MatDef mat_canon = {};
	mat_canon.Color = vec3f(0.8f, 0.8f, 0.8f);
	mat_canon.SpecularColor = vec3f(1.0f, 1.0f, 1.0f);
	mat_canon.SpecularExp = 56.0f;

	BD_MatDef mat_terrain = {};
	mat_terrain.Color = vec3f(0.45f, 0.25f, 0.05f);
	mat_terrain.SpecularColor = vec3f(1.0f, 1.0f, 1.0f);
	mat_terrain.SpecularExp = 2.0f;

	EXP_StaticMesh* terrain = new EXP_StaticMesh(game, "/terrain.msh", mat_terrain, vec3f(), vec3f(0.0f, 0.0f, 0.0f), vec3f(5.0f, 5.0f, 5.0f));

	EXP_StaticMesh* tonneau1 = new EXP_StaticMesh(game, "/Tonneau/tonneau_corps.msh", mat_tonneau, vec3f(0.0f, 1.2f, 0.4f), vec3f(24.0f, 0.0f, 0.0f), vec3f(0.2f, 0.2f, 0.2f));

	EXP_StaticMesh* tonneau2 = new EXP_StaticMesh(game, "/Tonneau/tonneau_corps.msh", mat_tonneau, vec3f(-1.5f, -2.0f, 0.2f), vec3f(-24.0f, 0.0f, 12.0f), vec3f(0.2f, 0.2f, 0.2f));

	EXP_StaticMesh* tonneau3 = new EXP_StaticMesh(game, "/Tonneau/tonneau_corps.msh", mat_tonneau, vec3f(-1.5f, 1.5f, 0.2f), vec3f(-12.0f, 0.0f, 12.0f), vec3f(0.2f, 0.2f, 0.2f));

	EXP_StaticMesh* epave = new EXP_StaticMesh(game, "/epave.msh", mat_tonneau, vec3f(-1.2f, 2.0f, 0.5f), vec3f(-120.0f, 0.0f, 0.0f), vec3f(0.125f, 0.5f, 0.25f));

	EXP_StaticMesh* canon = new EXP_StaticMesh(game, "/canon.msh", mat_canon, vec3f(-0.5f, -0.5f, 0.7f), vec3f(-190.0f, 30.0f, 0.0f), vec3f(0.2f, 0.2f, 0.2f));

	EXP_PointLight* testLight = new EXP_PointLight(game, vec3f(0.0f, 0.0f, 2.0f), vec3f(), vec3f(), vec3f(1.0f, 0.0f, 1.0f), 1.0f);
	EXP_PointLight* testLight2 = new EXP_PointLight(game, vec3f(-2.5f, 0.0f, 2.0f), vec3f(), vec3f(), vec3f(0.0f, 0.0f, 1.0f), 1.5f);
	EXP_PointLight* testLight3 = new EXP_PointLight(game, vec3f(2.5f, 0.0f, 0.5f), vec3f(), vec3f(), vec3f(1.0f, 0.0f, 0.0f), 1.5f);

	EXP_PointLight* testLight4 = new EXP_PointLight(game, vec3f(0.0f, 2.5f, 2.0f), vec3f(), vec3f(), vec3f(1.0f, 1.0f, 0.0f), 1.5f);
	EXP_PointLight* testLight5 = new EXP_PointLight(game, vec3f(0.0f, -2.5f, 1.0f), vec3f(), vec3f(), vec3f(0.0f, 1.0f, 1.0f), 1.5f);

	EXP_Camera* cam = new EXP_Camera(game, vec3f(-4.0f, 0.0f, 2.0f), vec3f(), vec3f(), vec3f(), 60.0f);
	cam->UseCamera();



	while (!game->GetRenderer()->WantToClose()) {
		game->MainLoop();
	}

	return 0;
}