#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <pReader.h>

#include <vec3.h>

#include <glm/glm.hpp>

#include "EXP_Game.h"
#include "EXP_Camera.h"
#include "EXP_PointLight.h"
#include "EXP_StaticMesh.h"

#include "BD_StructMan.h"
#include "BD_ActorRW.h"

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

class EXP_Game;

class EXPGE_API EXP_Actor
{
public:
	EXP_Actor(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, std::string actordef_path);
	~EXP_Actor();

	vec3f GetWorldPos();
	vec3f GetWorldRot();
	vec3f GetWorldScale();

	glm::mat4 CalculateActorMatrix();
	
private:
	EXP_Game* m_game;

	void LoadDef(std::string actordef);

	vec3f m_pos;
	vec3f m_rot;
	vec3f m_scale;

	std::vector<BD_ComponentReference> m_refs;

	std::vector<EXP_StaticMesh*> m_comp_meshes;
	std::vector<EXP_Camera*> m_comp_cam;
	std::vector<EXP_PointLight*> m_compt_pt_light;
};

