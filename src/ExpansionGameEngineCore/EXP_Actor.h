#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include <vec3.h>

#include <glm/glm.hpp>

#include "EXP_Game.h"
#include "EXP_Callbacks.h"

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
	~EXP_Actor();

	vec3f GetWorldPos();
	vec3f GetWorldRot();
	vec3f GetWorldScale();

	virtual void Tick();
	virtual void Start();
	virtual void Unregister();

	void UpdateActor();

	glm::mat4 GenerateActorMatrix();
	
protected:
	EXP_Game* m_game;

	vec3f m_pos;
	vec3f m_rot;
	vec3f m_scale;
};

