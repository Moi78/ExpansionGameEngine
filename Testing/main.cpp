#include <iostream>

#include <EXP_Game.h>
#include <EXP_StaticMesh.h>
#include <EXP_Camera.h>
#include <EXP_PointLight.h>

#include <vec3.h>
#include <BD_StructMan.h>
#include <BD_MatRW.h>

int main(int argc, char* argv[]) {
	BD_GameInfo gi = {};
	gi.RootEngineContentFolder = "Engine";
	gi.RootGameContentFolder = "Content";

	EXP_Game* game = new EXP_Game({ 1280, 720 }, gi, vec3f(0.0f, 0.41f, 0.54f), "Demo Scene");
	game->GetRenderer()->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	game->GetRenderer()->SetAmbientStrength(0.2f);

	EXP_StaticMesh* lame = new EXP_StaticMesh(game, "/epee/epee_lame", game->FetchMaterialFromFile("/epee/lame"), vec3f(0.0f, 0.0f, 1.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.15f, 0.5f, 1.5f));

	EXP_StaticMesh* manche = new EXP_StaticMesh(game, "/epee/epee_manche", game->FetchMaterialFromFile("/epee/manche"), vec3f(0.0f, 0.0f, -0.5f), vec3f(90.0f, 0.0f, 0.0f), vec3f(0.003f, 0.0015f, 0.003f));

	EXP_PointLight* light = new EXP_PointLight(game, vec3f(-3.0f, 0.0f, 0.0f), vec3f(), vec3f(), vec3f(1.0f, 1.0f, 1.0f), 4.0f);

	EXP_Camera* cam = new EXP_Camera(game, vec3f(-6.0f, 0.0f, 2.0f), vec3f(), vec3f(), vec3f(), 60.0f);
	cam->UseCamera();

	while (!game->GetRenderer()->WantToClose()) {
		cam->UpdateCamera();
		game->MainLoop();

		lame->SetRotation(lame->GetRotation() + vec3f(0.0f, 0.0f, 2.0f));
		lame->Update();

		manche->SetRotation(manche->GetRotation() + vec3f(0.0f, 2.0f, 0.0f));
		manche->Update();
	}

	return 0;
}