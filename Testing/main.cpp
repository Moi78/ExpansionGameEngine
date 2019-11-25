#include <iostream>

#include <RaindropRenderer.h>
#include <RD_ShaderLoader.h>
#include <RD_Mesh.h>
#include <RD_Camera.h>

#include <BD_Reader.h>

#include <glm/glm.hpp>

#include <Windows.h>

int main(int argc, char* argv) {
	//Expansion Game Engine Side
	RaindropRenderer* rndr = new RaindropRenderer(1280, 720, "Test Scn");

	rndr->EnableFeature(RendererFeature::Specular);

	rndr->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	rndr->SetAmbientStrength(0.2f);

	RD_Camera* cam = new RD_Camera(rndr->GetShader(), rndr, 60.0f, 0.1f, 100.0f, vec3f(-5.5f, -5.5f, 4.0f), vec3f(0.0f, 0.0f, 0.0f));
	cam->UseCamera();

	BD_MatDef mat = {};
	mat.Color = vec3f(0.70f, 0.50f, 0.30f);

	BD_MatDef mat_island = {};
	mat_island.Color = vec3f(1.0f, 1.0f, 0.0f);

	BD_MatDef mat_sea = {};
	mat_sea.Color = vec3f(0.0f, 0.1f, 1.0f);

	BD_MatDef mat_leaves = {};
	mat_leaves.Color = vec3f(0.0f, 0.5f, 0.0f);

	RD_Mesh* sphere = new RD_Mesh(rndr->GetShader(), mat, vec3f(0.5f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 45.0f), vec3f(0.3f, 0.3f, 0.5f));
	sphere->loadMesh("trunk.msh");

	RD_Mesh* island = new RD_Mesh(rndr->GetShader(), mat_island, vec3f(), vec3f(), vec3f(2.0f, 2.0f, 0.5f));
	island->loadMesh("island.msh");

	RD_Mesh* sea = new RD_Mesh(rndr->GetShader(), mat_sea, vec3f(), vec3f(), vec3f(1.0f, 1.0f, 1.0f));
	sea->loadMesh("sea.msh");

	RD_Mesh* leaves = new RD_Mesh(rndr->GetShader(), mat_leaves, vec3f(-0.2f, 1.2f, 4.0f), vec3f(), vec3f(0.3f, 1.0f, 0.15f));
	leaves->loadMesh("leaves.msh");

	RD_PointLight* light = new RD_PointLight(vec3f(-2.0f, -2.0f, 4.0f), vec3f(1.0f, 1.0f, 1.0f), 2.5f);
	rndr->AppendLight(light);

	while (!rndr->WantToClose()) {
		rndr->ClearWindow(vec3f(0.0f, 0.3f, 0.4f));

		if (glfwGetKey(rndr->GetGLFWwindow(), GLFW_KEY_UP) == GLFW_PRESS) {
			cam->TranslateCamera(vec3f(0.1f, 0.0f, 0.0f));
		}

		if (glfwGetKey(rndr->GetGLFWwindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
			cam->TranslateCamera(vec3f(-0.1f, 0.0f, 0.0f));
		}

		if (glfwGetKey(rndr->GetGLFWwindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
			cam->TranslateCamera(vec3f(0.0f, -0.2f, 0.0f));
		}

		if (glfwGetKey(rndr->GetGLFWwindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
			cam->TranslateCamera(vec3f(0.0f, 0.2f, 0.0f));
		}

		//rndr->RenderDbg();

		island->render();
		sphere->render();
		sea->render();
		leaves->render();

		cam->UpdateCamera();
		rndr->SwapWindow();
	}

	delete rndr;

	return 0;
}