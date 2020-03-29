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
#include <BD_ActorRW.h>

int main(int argc, char* argv[]) {
	BD_GameInfo gi = {};
	gi.RootEngineContentFolder = "Engine";
	gi.RootGameContentFolder = "Content";
	gi.GameName = "Demo Scene";

	EXP_Game* game = new EXP_Game({ 1280, 720 }, gi, vec3f(0.0f, 0.41f, 0.54f));
	game->GetRenderer()->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	game->GetRenderer()->SetAmbientStrength(0.1f);

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

	EXP_StaticMesh* shape = new EXP_StaticMesh(game, "/shape", matShape, vec3f(0.0f, 0.0f, 12.0f), vec3f(), vec3f(1.0f, 1.0f, 1.0f));

	EXP_Camera* cam = new EXP_Camera(game, vec3f(-8.0f, 3.0f, 4.0f), vec3f(), vec3f(), vec3f(0.0f, 0.0f, 0.0f), 60.0f);
	cam->Use();

	EXP_PointLight* light = new EXP_PointLight(game, vec3f(0.0f, 0.0f, 13.0f), vec3f(), vec3f(), vec3f(1.0f, 1.0f, 1.0f), 600.0f);

	EXP_RB_Box* rb = new EXP_RB_Box(game, vec3f(0.0f, 0.0f, 12.0f), vec3f(20.0f, 60.0f, 30.0f), vec3f(1.0f, 1.0f, 1.0f), 40.0f, vec3f(2.0f));

	EXP_RB_Box* rbfloor = new EXP_RB_Box(game, vec3f(0.0f, 0.0f, 0.0f), vec3f(), vec3f(10.0f, 10.0f, 0.5f), 0.0f);

	while (!game->GetRenderer()->WantToClose()) {
		if (glfwGetKey(game->GetRenderer()->GetGLFWwindow(), GLFW_KEY_UP) == GLFW_PRESS) {
			cam->Translate(vec3f(0.1f), true);
			cam->UpdateCamera();
		}

		if (glfwGetKey(game->GetRenderer()->GetGLFWwindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
			cam->Translate(vec3f(-0.1f), true);
			cam->UpdateCamera();
		}

		game->MainLoop();

		shape->SetPosition(rb->GetWorldPosition());
		shape->Update();
	}

	delete game;

	return 0;
}