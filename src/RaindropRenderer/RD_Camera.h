#pragma once

#ifndef _RD_CAMERA_H__
#define _RD_CAMERA_H__

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#else

#define RAINDROPRENDERER_API

#endif //_WIN32

#include "RaindropRenderer.h"
#include "RD_ShaderLoader.h"
#include "RD_RenderingAPI.h"

#include <mat4.h>
#include <quat.h>
#include <vec3.h>

#include <iostream>
#include <string>
#include <algorithm>

class RAINDROPRENDERER_API RD_Camera
{
public:
	RD_Camera(RaindropRenderer* rndr , float FOVinDegrees, float CamNear, float CamFar, vec3f position, vec3f YawPitchRoll, bool noInitUpdt = false);
	~RD_Camera();

	void UseCamera(RD_ShaderLoader* shader);

	void SetLocation(vec3f position);
	void SetSubject(vec3f lookingAt);
	void SetFOV(float FOVinDegrees);

	vec3f GetSubject();
	
	void UpdateCamera();
	void UpdateView();
	void UpdateProj();
	
	virtual void RotateCamera(vec3f rotation);
	virtual void TranslateCamera(vec3f translation, bool changeSub = false);

	vec3f GetForwardVector();
	vec3f GetRightVector();

	vec3f GetLocation();
	vec3f GetYPR();

	virtual void AddPitch(float pitch);
	virtual void AddYaw(float yaw);
	virtual void AddRoll(float roll);
	virtual void SetYPR(vec3f YawPitchRoll);
private:
	void ComputeYPR();

	mat4f projection;
	mat4f view;

	RaindropRenderer* m_rndr;

	RD_UniformBuffer* m_mat;
	RD_UniformBuffer* m_camPos;

	float FOV;
	float m_near;
	float m_far;

	vec3f m_pos;
	vec3f m_subject;
	vec3f m_up;

	vec3f m_yawPitchRoll;
};

#endif //_RD_CAMERA_H__
