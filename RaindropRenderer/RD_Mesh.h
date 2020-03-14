#pragma once

#ifndef _RD_MESH_H__
#define _RD_MESH_H__

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#include "RD_ShaderLoader.h"
#include "RD_Materials.h"

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
	RD_Mesh(RD_ShaderLoader* shader, BD_MatDef material, vec3f position, vec3f rotation, vec3f scale);
	~RD_Mesh();

	void loadMesh(std::string);
	void render(RenderMode rndrMode = RenderMode::Filled);

	void addRotation(vec3f rotation);
	void addTranslation(vec3f translation);
	void addScale(vec3f scale);

	void SetRotation(vec3f nRotation);
	void SetPosition(vec3f nPos);
	void SetScale(vec3f nScale);

	void UpdateMaterial(BD_MatDef*);

	vec3f GetLocation();

	void ApplyActorMatrix(glm::mat4);
	void SetActorMode(bool mode);

	std::vector<double> RAWvertices;
	std::vector<unsigned int> RAWindices;
	std::vector<double> RAWnormals;
	std::vector<double> RAWuv;

	std::vector<double> MixVertNorm;

	int m_nbr_vertices;

	glm::mat4 m_actor_mat;
	bool inActor;
	
private:
	void Bufferize();

	unsigned int VAO, VBO, EBO;

	RD_SimpleMaterial* m_mat;
	RD_ShaderLoader* m_shader;

	vec3f m_position;
	vec3f m_rotation;
	vec3f m_scale;
};

#endif