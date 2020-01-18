#pragma once

#include <iostream>
#include <string>

#include <vec3.h>

#include "EXP_Game.h"

class EXP_Actor
{
public:
	EXP_Actor(vec3f pos, vec3f rot, vec3f scale);
	~EXP_Actor();

	vec3f GetWorldPos();
	vec3f GetWorldRot();
	vec3f GetWorldScale();
private:
	vec3f m_pos;
	vec3f m_rot;
	vec3f m_scale;
};

