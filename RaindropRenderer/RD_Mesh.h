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

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include <BD_Reader.h>
#include <BulldozerFileManager.h>
#include <BD_StructMan.h>

#include <vec3.h>

class RAINDROPRENDERER_API RD_Mesh
{
public:
	RD_Mesh(BD_MatDef material, vec3f position, vec3f rotation, vec3f scale);
	~RD_Mesh();

	void loadMesh(std::string);

	void render(RD_ShaderLoader* shader, RenderMode rndrMode = RenderMode::Filled);
	void renderShadows(RD_ShaderLoader* shadowShader);

	void addRotation(vec3f rotation);
	void addTranslation(vec3f translation);
	void addScale(vec3f scale);

	void SetRotation(vec3f nRotation);
	void SetPosition(vec3f nPos);
	void SetScale(vec3f nScale);

	vec3f GetLocation();

	void ApplyActorMatrix(glm::mat4);
	void SetActorMode(bool mode);

	glm::mat4 m_actor_mat;
	bool inActor;
	
private:
	void Bufferize();

	unsigned int VAO, VBO, EBO;

	std::vector<vec3d> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<vec3d> m_normals;
	std::vector<vec2d> m_uv;

	int m_nbr_indices;
	int m_nbr_vertices;

	std::vector<double> MixVertNormUV;

	RD_SimpleMaterial* m_mat;

	vec3f m_position;
	vec3f m_rotation;
	vec3f m_scale;
};

#endif