#pragma once

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RD_API __declspec(dllexport)
	#else
		#define RD_API __declspec(dllimport)
	#endif //RAINDROPRENDERER_EXPORTS
#else
	#define RD_API
#endif //_WIN32

#include "RaindropRenderer.h"
#include "RD_Mesh.h"
#include "RD_RenderingAPI.h"

#include <vector>

class RD_API RD_MeshBuilder : private RD_Mesh
{
public:
	RD_MeshBuilder(RaindropRenderer* rndr, RD_ShaderMaterial* shader, vec3f position, vec3f rotation, vec3f scale);
	~RD_MeshBuilder();

	void LoadMeshBuffer(std::vector<vec3f> pos, std::vector<vec3f> norm, std::vector<vec2f> UV, std::vector<unsigned int> indices);

	RD_Mesh* GetMesh();
};

