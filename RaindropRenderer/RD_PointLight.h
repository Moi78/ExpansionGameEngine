#pragma once

#ifndef _RD_POINT_LIGHT_H__
#define _RD_POINT_LIGHT_H__

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#include <iostream>

#include <vec3.h>

class RAINDROPRENDERER_API RD_PointLight
{
public:
	RD_PointLight(vec3f position, float brightness);
	~RD_PointLight();

	void SetPosition(vec3f nPos);
	vec3f GetPosition();

	void SetBrightness(float nBrightness);
	float GetBrightness();

	int GetLightIndex();

private:
	void SetLightIndex(int nIndex);

	vec3f m_pos;
	float m_brightness;

	int m_lightIndex;
};

#endif