#pragma once

#ifndef _RD_CAMERA_H__
#define _RD_CAMERA_H__

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#include "RaindropRenderer.h"
#include "RD_ShaderLoader.h"

#include <glm/glm.hpp>

#include <iostream>
#include <string>

#include <vec3.h>
#include <rotator.h>

class RAINDROPRENDERER_API RD_Camera
{
public:
	RD_Camera(RD_ShaderLoader* shader, RaindropRenderer* rndr , float FOVinDegrees, float CamNear, float CamFar, vec3f position, vec3f lookingAt);
	~RD_Camera();

	void UseCamera();

	void SetPosition(vec3f position);
	void SetSubject(vec3f lookingAt);
	void SetFOV(float FOVinDegrees);

	void RotateCamera(rotator);
private:
	void SetupCamera();
	void UpdateCamera();

	glm::mat4 projection;
	glm::mat4 view;

	RD_ShaderLoader* m_shader;
	RaindropRenderer* m_rndr;

	float FOV;
	float m_near;
	float m_far;

	vec3f m_pos;
	vec3f m_subject;

	float m_pitch;
	float m_yaw;
	float m_roll;
};

#endif //_RD_CAMERA_H__