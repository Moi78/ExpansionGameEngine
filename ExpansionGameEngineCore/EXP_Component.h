#pragma once

#ifndef _EXP_COMPONENT_H__
#define _EXP_COMPONENT_H__

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#include <string>

#include <vec3.h>

class EXPGE_API EXP_Component
{
public:
	EXP_Component(vec3f pos, vec3f rot, vec3f scale);
	~EXP_Component();

	vec3f GetPosition();
	vec3f GetRotation();
	vec3f GetScale();

	void SetPosition(vec3f nPos);
	void SetRotation(vec3f nRot);
	void SetScale(vec3f nScale);

private:
	vec3f m_pos;
	vec3f m_rot;
	vec3f m_scale;
};

#endif //_EXP_COMPONENT_H__