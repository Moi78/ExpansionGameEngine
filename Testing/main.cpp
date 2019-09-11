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

	rndr->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	rndr->SetAmbientStrength(0.1f);

	BD_MatDef mat = {};
	mat.Color = vec3f(1.0f, 0.0f, 0.0f);

	BD_MatDef mat2 = {};
	mat2.Color = vec3f(0.1f, 1.0f, 0.0f);


	RD_Mesh* mesh = new RD_Mesh(rndr->GetShader(), mat, vec3f(-3.0f, -3.0f, 1.0f), vec3f(0.0f, 0.0f, 100.0f), vec3f(0.5f, 0.5f, 0.5f));
	mesh->loadMesh("model.msh");

	RD_Mesh* mesh2 = new RD_Mesh(rndr->GetShader(), mat2, vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 180.0f), vec3f(5.0f, 5.0f, 5.0f));
	mesh2->loadMesh("model2.msh");

	RD_PointLight* pt = new RD_PointLight(vec3f(-4.0f, -3.0f, 2.0f), 0.5f);
	rndr->AppendLight(pt);

	RD_Camera* cam = new RD_Camera(rndr->GetShader(), rndr, 60.0f, 0.1f, 100.0f, vec3f(-10.0f, 0.0f, 5.0f), vec3f(0.0f, 0.0f, 0.0f));
	cam->UseCamera();

	while (!rndr->WantToClose()) {
		rndr->ClearWindow(vec3f(0.0f, 0.0f, 0.0f));

		rndr->RenderDbg();

		mesh2->render();
		mesh->render();

		glfwSetInputMode(rndr->GetGLFWwindow(), GLFW_STICKY_KEYS, GLFW_TRUE);
		if (glfwGetKey(rndr->GetGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS) {
			mat2.Color = vec3f(1.0f, 1.0f, 0.0f);

			mesh2->UpdateMaterial(&mat2);
		}
		else if (glfwGetKey(rndr->GetGLFWwindow(), GLFW_KEY_Z) == GLFW_PRESS) {
			mat2.Color = vec3f(1.0f, 0.0f, 0.5f);

			mesh2->UpdateMaterial(&mat2);
		}

		rndr->SwapWindow();
	}

	delete rndr;
	delete mesh;

	return 0;
}