#pragma once

#ifndef _EXP_CAMERA_H__
#define _EXP_CAMERA_H__

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

#include <RD_Camera.h>

#include <vec3.h>

class EXPGE_API EXP_Camera : public EXP_Component, public RD_Camera
{
public:
	EXP_Camera(EXP_Game* gameinstance, vec3f pos, vec3f rot, vec3f scale, vec3f YawPitchRoll, float FOV, float Near = 0.1f, float Far = 100.0f, bool inheritParentRot = false);
	~EXP_Camera();

	void Translate(vec3f translation, bool changeSubject = false);
	void Use();

	//EXP_Component virtual methods override
	virtual vec3f GetPosition() override;
	virtual vec3f GetRotation() override;
	virtual vec3f GetScale() override;

	virtual void SetPosition(vec3f nPos) override;
	virtual void SetRotation(vec3f YawPitchRoll) override;

	virtual void UseParentMatrix(glm::mat4 mat) override;
private:
	void ComputeCamRealCoord();

	EXP_Game* m_gameinstance;
	bool m_inheritParentRot;
};

#endif //_EXP_CAMERA_H__