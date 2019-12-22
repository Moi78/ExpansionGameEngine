#pragma once

#ifndef _EXP_POINT_LIGHT_H__
#define _EXP_POINT_LIGHT_H__

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#include "EXP_Component.h"
#include "EXP_Game.h"

#include <vec3.h>

#include <RD_PointLight.h>

class EXPGE_API EXP_PointLight : public EXP_Component
{
public:
	EXP_PointLight(EXP_Game* gameinstance, vec3f pos, vec3f rot, vec3f scale, vec3f color, float brightness);
	~EXP_PointLight();

private:
	void RegisterLightToGame();

	RD_PointLight* m_light;
	EXP_Game* m_gameinstance;
};

#endif //_EXP_POINT_LIGHT_H__