#include <iostream>

#include <EXP_Game.h>
#include <EXP_StaticMesh.h>
#include <EXP_Camera.h>
#include <EXP_PointLight.h>
#include <EXP_SoundEmitter.h>
#include <EXP_RigidBody.h>
#include <EXP_PhysicsHandler.h>

#include <RD_GUI.h>
#include <RD_Callback.h>

#include <PhysicaSound.h>

#include <vec3.h>

#include <BD_StructMan.h>

#include "MainCharacter.h"

int main(int argc, char* argv[]) {
	BD_GameInfo gi = {};
	gi.RootEngineContentFolder = "Engine";
	gi.RootGameContentFolder = "Content";
	gi.GameName = "Demo Scene";

	EXP_Game* game = new EXP_Game({ 1280, 720 }, gi, vec3f(0.0f, 0.41f, 0.54f));
	game->GetRenderer()->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	game->GetRenderer()->SetAmbientStrength(0.1f);

	MainCharacter* chara = new MainCharacter(game, vec3f(-5.0f, -5.0f, 0.5f));

	RD_Texture* color = new RD_Texture();
	color->GenerateColorTex(vec3f(1.0f, 0.0f, 1.0f));

	RD_Texture* colorShape = new RD_Texture();
	colorShape->GenerateColorTex(vec3f(0.8f, 0.8f, 0.8f));

	BD_MatDef mat = {};
	mat.BaseColor = color->GetTextureID();
	mat.SpecularColor = vec3f(1.0f, 1.0f, 1.0f);
	mat.SpecularStrength = 1.0f;
	mat.Shininess = 128.0f;

	BD_MatDef matShape = {};
	matShape.BaseColor = colorShape->GetTextureID();
	matShape.SpecularColor = vec3f(1.0f, 1.0f, 1.0f);
	matShape.SpecularStrength = 1.0f;
	matShape.Shininess = 128.0f;

	EXP_StaticMesh* floor = new EXP_StaticMesh(game, "/floor", mat, vec3f(), vec3f(), vec3f(10.0f, 10.0f, 0.5f));

	EXP_PointLight* light = new EXP_PointLight(game, vec3f(0.0f, 0.0f, 13.0f), vec3f(), vec3f(), vec3f(1.0f, 1.0f, 1.0f), 600.0f);

	EXP_RB_Box* rbfloor = new EXP_RB_Box(game, vec3f(0.0f, 0.0f, 0.0f), vec3f(), vec3f(10.0f, 10.0f, 0.5f), 0.0f);

	while (!game->GetRenderer()->WantToClose()) {
		chara->UpdateCharacter();

		game->MainLoop();
	}

	delete game;

	return 0;
}