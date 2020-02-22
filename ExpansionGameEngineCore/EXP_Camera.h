#pragma once

#ifndef _EXP_CAMERA_H__
#define _EXP_CAMERA_H__

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#include "EXP_Component.h"
#include "EXP_Game.h"

#include <RD_Camera.h>

#include <vec3.h>

class EXPGE_API EXP_Camera : public EXP_Component, public RD_Camera
{
public:
	EXP_Camera(EXP_Game* gameinstance, vec3f pos, vec3f rot, vec3f scale, vec3f dir, float FOV, float Near = 0.1f, float Far = 100.0f);
	~EXP_Camera();

	void Translate(vec3f translation, bool changeSubject = false);

	void Use();

private:
	EXP_Game* m_gameinstance;
};

#endif //_EXP_CAMERA_H__