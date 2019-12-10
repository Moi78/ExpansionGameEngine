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

class EXPGE_API EXP_Camera : public EXP_Component, RD_Camera
{
public:
	EXP_Camera();
	~EXP_Camera();
};

#endif //_EXP_CAMERA_H__