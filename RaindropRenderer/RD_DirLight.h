#pragma once

#ifndef _DIR_LIGHT_H__
#define _DIR_LIGHT_H__

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#include <iostream>

#include <vec3.h>

class RAINDROPRENDERER_API RD_DirLight
{
public:
	RD_DirLight(vec3f dir, vec3f color, float brightness);
	~RD_DirLight();

	void SetLightDir(vec3f nDir);
	vec3f GetLightDir();

	void SetLightColor(vec3f nColor);
	vec3f GetLightColor();

	void SetLightBrightness(float nBrightness);
	float GetBrightness();

private:
	vec3f m_dir;
	vec3f m_color;
	float m_brightness;
};

#endif //_DIR_LIGHT_H__