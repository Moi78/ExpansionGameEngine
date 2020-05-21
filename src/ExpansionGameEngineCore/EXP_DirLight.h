#pragma once

#ifndef _EXP_DIR_LIGHT_H__
#define _EXP_DIR_LIGHT_H__

#ifdef _WIN32

	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif

#else

	#define EXPGE_API

#endif

#include <RaindropRenderer.h>
#include <RD_DirLight.h>

#include <vec3.h>

#include "EXP_Game.h"
#include "EXP_Component.h"

class EXPGE_API EXP_DirLight :
	public RD_DirLight,
	public EXP_Component
{
public:
	EXP_DirLight(EXP_Game* game, vec3f dir, vec3f color, float brightness);
	~EXP_DirLight();
};

#endif