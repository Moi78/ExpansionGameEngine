#pragma once

#ifndef RD_INSTANCIATED_MESH_H__
#define RD_INSTANCIATED_MESH_H__

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RD_API __declspec(dllexport)
	#else
		#define RD_API __declspec(dllimport)
	#endif
#else
	#define RD_API
#endif

#include <vector>

#include "RaindropRenderer.h"

#include "RD_Mesh.h"
#include "RD_Materials.h"

struct RD_MeshInstance {
	vec3f pos;
	vec3f rotation;
	vec3f scale;
};

class RD_API RD_InstanciatedMesh : public RD_Mesh
{
public:
	RD_InstanciatedMesh(RaindropRenderer* rndr, RD_ShaderMaterial* material);
	~RD_InstanciatedMesh();

	int AppendInstance(RD_MeshInstance inst);

	virtual void render(RD_Camera* cam) override;
	virtual void renderShadows(RD_ShaderLoader* shadowShader) override;

	virtual void addRotation(vec3f rotation, bool update = true) override;
	virtual void addTranslation(vec3f translation, bool update = true) override;
	virtual void addScale(vec3f scale, bool update = true) override;

	void SetInstRotation(vec3f nRotation, int index, bool update = true);
	void SetInstPosition(vec3f nPos, int index, bool update = true);
	void SetInstScale(vec3f nScale, int index, bool update = true);

	void AddInstRotation(vec3f Rotation, int index, bool update = true);
	void AddInstTranslation(vec3f Pos, int index, bool update = true);
	void AddInstScale(vec3f Scale, int index, bool update = true);

	virtual void Update() override;

private:
	void UpdateIndex(int index);

	std::vector<std::pair<mat4f, RD_MeshInstance>> m_mdls;
};

#endif //RD_INSTANCIATED_MESH_H__