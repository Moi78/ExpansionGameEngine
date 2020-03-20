#include <iostream>

#include <EXP_Game.h>
#include <EXP_StaticMesh.h>
#include <EXP_Camera.h>
#include <EXP_PointLight.h>
#include <EXP_SoundEmitter.h>
#include <EXP_RigidBody.h>

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

	EXP_Game* game = new EXP_Game({ 1280, 720 }, gi, vec3f(0.0f, 0.41f, 0.54f), "Demo Scene");
	game->GetRenderer()->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	game->GetRenderer()->SetAmbientStrength(0.1f);

	EXP_Camera* cam = new EXP_Camera(game, vec3f(-8.0f, 3.0f, 4.0f), vec3f(), vec3f(), vec3f(0.0f, 0.0f, 0.0f), 60.0f);
	cam->Use();

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
	}

	delete game;

	return 0;
}