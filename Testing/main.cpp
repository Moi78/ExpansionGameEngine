#include <iostream>

#include <RaindropRenderer.h>
#include <RD_ShaderLoader.h>
#include <RD_Mesh.h>
#include <RD_Camera.h>

#include <BD_Reader.h>

#include <glm/glm.hpp>

#include <Windows.h>

int main(int argc, char* argv[]) {
	RaindropRenderer* rndr = new RaindropRenderer(1280, 720, "Hello World");

	BD_MatDef mat = {};
	mat.Color = vec3f(1.0f, 0.0f, 0.0f);

	RD_Mesh* mesh = new RD_Mesh(rndr->GetShader(), mat, vec3f(0.0f, 5.0f, 0.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(1.0f, 1.0f, 1.0f));
	mesh->loadMesh("platform.msh");

	BD_MatDef mat2 = {};
	mat.Color = vec3f(0.0f, 1.0f, 0.0f);

	RD_Mesh* mesh2 = new RD_Mesh(rndr->GetShader(), mat, vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.5f, 0.5f, 0.25f));
	mesh2->loadMesh("test.msh");

	RD_Camera* cam = new RD_Camera(rndr->GetShader(), rndr, 60.0f, 0.1f, 100.0f, vec3f(-5.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, -0.5f));
	cam->UseCamera();

	while (!rndr->WantToClose()) {
		rndr->ClearWindow(vec3f(0.0f, 0.0f, 0.0f));

		mesh->render();

		mesh2->addRotation(vec3f(0.1f, 0.0f, 0.1f));
		mesh2->render();

		rndr->SwapWindow();
	}

	delete rndr;
	delete mesh;

	return 0;
}