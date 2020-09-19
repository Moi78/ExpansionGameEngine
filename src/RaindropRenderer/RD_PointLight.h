#pragma once

#ifndef _RD_POINT_LIGHT_H__
#define _RD_POINT_LIGHT_H__

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#else

#define RAINDROPRENDERER_API

#endif //_WIN32

#include <iostream>

#include <vec3.h>

class RAINDROPRENDERER_API RD_PointLight
{
public:
	RD_PointLight(vec3f position, vec3f color, float brightness, float radius);
	virtual ~RD_PointLight();

	void SetPosition(vec3f nPos);
	vec3f GetPosition();

	void SetBrightness(float nBrightness);
	float GetBrightness();

	void SetColor(vec3f color);
	vec3f GetColor();

	void SetLightRadius(float nRad);
	float GetLightRadius();

	int GetLightIndex();

private:
	void SetLightIndex(int nIndex);

	vec3f m_pos;
	vec3f m_color;
	float m_brightness;
	float m_radius;

	int m_lightIndex;
};

#endif