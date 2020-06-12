#pragma once

#ifndef _RD_MESH_H__
#define _RD_MESH_H__

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#else

#define RAINDROPRENDERER_API

#endif //_WIN32

#include "RD_ShaderLoader.h"
#include "RD_Materials.h"
#include "RaindropRenderer.h"
#include "RD_Camera.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>

#include <BD_Reader.h>
#include <BulldozerFileManager.h>
#include <BD_StructMan.h>

#include <vec3.h>

class RAINDROPRENDERER_API RD_Mesh
{
public:
	RD_Mesh(RD_ShaderMaterial* shader, vec3f position, vec3f rotation, vec3f scale);
	~RD_Mesh();

	void loadMesh(std::string);

	void render(RD_Camera* cam, RenderMode rndrMode = RenderMode::Filled);
	void renderShadows(RD_ShaderLoader* shadowShader);

	void addRotation(vec3f rotation);
	void addTranslation(vec3f translation);
	void addScale(vec3f scale);

	void SetRotation(vec3f nRotation);
	void SetPosition(vec3f nPos);
	void SetScale(vec3f nScale);

	void SetWorldPosition(vec3f nPos);
	void SetWorldRotation(vec3f nRot);
	void SetWorldScale(vec3f nScale);

	vec3f GetLocation();

	RD_ShaderMaterial* GetMaterial();
	
private:
	void Bufferize();
	void Update();

	unsigned int VAO, VBO, EBO;

	std::vector<vec3f> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<vec3f> m_normals;
	std::vector<vec2f> m_uv;

	int m_nbr_indices;

	std::vector<float> MixVertNormUV;

	RD_ShaderMaterial* m_mat;

	vec3f m_position;
	vec3f m_rotation;
	vec3f m_scale;

	vec3f m_worldPos;
	vec3f m_worldRot;
	vec3f m_worldScale;

	glm::mat4 m_mdl;
};

#endif