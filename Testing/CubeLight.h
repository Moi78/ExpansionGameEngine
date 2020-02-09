#pragma once

#ifndef _CUBE_LIGHT_H__
#define _CUBE_LIGHT_H__

#include <EXP_Actor.h>
#include <EXP_Game.h>

class CubeLight :
	public EXP_Actor
{
public:
	CubeLight(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale);
	~CubeLight();
};

#endif _CUBE_LIGHT_H__