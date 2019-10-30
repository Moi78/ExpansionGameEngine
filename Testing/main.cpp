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
	RaindropRenderer* rndr = new RaindropRenderer(500, 500, "Material Editor");

	rndr->SetAmbientColor(vec3f(1.0f, 1.0f, 1.0f));
	rndr->SetAmbientStrength(0.1f);

	RD_Camera* cam = new RD_Camera(rndr->GetShader(), rndr, 60.0f, 0.1f, 100.0f, vec3f(-2.5f, 0.0f, 0.0f), vec3f());
	cam->UseCamera();

	BD_MatDef mat = {};
	mat.Color = vec3f(1.0f, 1.0f, 1.0f);

	RD_Mesh* sphere = new RD_Mesh(rndr->GetShader(), mat, vec3f(), vec3f(), vec3f(1.0f, 1.0f, 1.0f));
	sphere->loadMesh("sphere.msh");

	RD_PointLight* light = new RD_PointLight(vec3f(-2.0f, 1.0f, 1.0f), 1.5f);
	rndr->AppendLight(light);

	while (!rndr->WantToClose()) {
		rndr->ClearWindow(vec3f(0.0f, 0.0f, 0.0f));

		rndr->RenderDbg();

		sphere->render();

		cam->UpdateCamera();
		rndr->SwapWindow();
	}

	delete rndr;

	return 0;
}