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
	EXP_Actor(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, std::function<void()> tickFunc, std::function<void()> startFunc, std::function<void()> OnUnregister);
	~EXP_Actor();

	vec3f GetWorldPos();
	vec3f GetWorldRot();
	vec3f GetWorldScale();

	void CallTick();
	void CallOnStart();
	void CallUnregister();

	void UpdateActor();

	glm::mat4 GenerateActorMatrix();
	
protected:
	EXP_Game* m_game;

	vec3f m_pos;
	vec3f m_rot;
	vec3f m_scale;

	std::unique_ptr<RD_Callback> m_tick;
	std::unique_ptr<RD_Callback> m_onStart;
	std::unique_ptr<RD_Callback> m_onUnregister;
};

