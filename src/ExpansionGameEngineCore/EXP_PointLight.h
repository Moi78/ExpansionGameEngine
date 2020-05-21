#pragma once

#ifndef _EXP_POINT_LIGHT_H__
#define _EXP_POINT_LIGHT_H__

#ifdef _WIN32

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#else

#define EXPGE_API

#endif //_WIN32

#include "EXP_Component.h"
#include "EXP_Game.h"

#include <vec3.h>

#include <RD_PointLight.h>

class EXPGE_API EXP_PointLight :
	public EXP_Component,
	public RD_PointLight
{
public:
	EXP_PointLight(EXP_Game* gameinstance, vec3f pos, vec3f color, float brightness, float radius);
	~EXP_PointLight();

	virtual void SetPosition(vec3f nPos) override;
	virtual vec3f GetPosition() override;

private:
	void RegisterLightToGame();

	EXP_Game* m_gameinstance;
};

#endif //_EXP_POINT_LIGHT_H__