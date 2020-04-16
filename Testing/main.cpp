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
#include <RD_DirLight.h>

#include <PhysicaSound.h>

#include <vec3.h>

#include <BD_StructMan.h>

#include "MainCharacter.h"
#include "Donut.h"

int main(int argc, char* argv[]) {
	BD_GameInfo gi = {};
	gi.RootEngineContentFolder = "Engine";
	gi.RootGameContentFolder = "Content";
	gi.GameName = "Demo Scene";

	EXP_Game* game = new EXP_Game({ 1280, 720 }, gi, vec3f(0.0f, 0.41f, 0.54f));
	game->GetRenderer()->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	game->GetRenderer()->SetAmbientStrength(0.1f);

	game->GetRenderer()->SetAASampling(8);

	RD_DirLight* dlight = new RD_DirLight(vec3f(1.0f, 0.0, -0.5f), vec3f(1.0f, 1.0f, 1.0f), 1.0f);
	game->GetRenderer()->AppendDirLight(dlight);
	
	RD_DirLight* dlight2 = new RD_DirLight(vec3f(0.0f, 1.0f, -0.5f), vec3f(1.0f, 1.0f, 1.0f), 1.0f);
	game->GetRenderer()->AppendDirLight(dlight2);

	RD_DirLight* dlight3 = new RD_DirLight(vec3f(0.0f, -1.0f, -0.5f), vec3f(1.0f, 1.0f, 1.0f), 1.0f);
	game->GetRenderer()->AppendDirLight(dlight3);

	MainCharacter* chara = new MainCharacter(game, vec3f(-5.0f, -5.0f, 0.5f));
	Donut* donut = new Donut(game, vec3f(0.0f, 0.0f, 2.5f));

	RD_Texture* color = new RD_Texture();
	color->GenerateColorTex(vec3f(1.0f, 0.2f, 0.3f));

	RD_Texture* colorShape = new RD_Texture();
	colorShape->GenerateColorTex(vec3f(0.8f, 0.8f, 0.8f));

	RD_Texture* spec = new RD_Texture();
	spec->GenerateColorTex(vec3f(1.0f, 1.0f, 1.0f));

	BD_MatDef mat = {};
	mat.BaseColor = color->GetTextureID();
	mat.Specular = spec->GetTextureID();
	mat.Shininess = 128.0f;

	EXP_StaticMesh* floor = new EXP_StaticMesh(game, "/floor", mat, vec3f(), vec3f(), vec3f(10.0f, 10.0f, 0.5f));

	EXP_RB_Box* rbfloor = new EXP_RB_Box(game, vec3f(0.0f, 0.0f, 0.0f), vec3f(), vec3f(10.0f, 10.0f, 0.5f), 0.0f);

	while (!game->GetRenderer()->WantToClose()) {
		game->MainLoop();
	}

	delete game;
	delete chara;
	delete donut;
	delete color;
	delete colorShape;
	delete spec;
	delete floor;
	delete rbfloor;

	return 0;
}