#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <vec3.h>

#include "EXP_Game.h"
#include "EXP_Callbacks.h"

#include <mat4.h>

#ifdef _WIN32

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#else

#define EXPGE_API

#endif //_WIN32

#include <vector>
#include <memory>

#include "EXP_Component.h"

//Forward decl
class EXP_Game;

class EXPGE_API EXP_Actor
{
public:
	EXP_Actor(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale);
	virtual ~EXP_Actor();

	vec3f GetWorldPos();
	vec3f GetWorldRot();
	vec3f GetWorldScale();

	void SetWorldPos(vec3f nPos);
	void SetWorldRot(vec3f nRot);
	void SetWorldScale(vec3f nScale);

	void AddWorldPos(vec3f pos_offset);
	void AddWorldRot(vec3f rot_offset);
	void AddWorldScale(vec3f scale_offset);

	virtual void OnTick();
	virtual void OnStart();
	virtual void OnUnregister();

	void UpdateActor();
	void UpdateActorMatrix();

	mat4f GenerateActorMatrix();
	void LinkComponent(EXP_Component* comp);
	
protected:
	EXP_Game* m_game;

	vec3f m_pos;
	vec3f m_rot;
	vec3f m_scale;
	
	mat4f m_actor_mat;

	std::vector<EXP_Component*> m_comps;
};

