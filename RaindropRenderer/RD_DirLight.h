#pragma once

#ifndef _DIR_LIGHT_H__
#define _DIR_LIGHT_H__

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

#include "RaindropRenderer.h"

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

	void DepthRender(RaindropRenderer*, vec3f CamPos);
	void SetUpShadowFB(unsigned int shadowQual);
	glm::mat4 GetLightSpace();
	unsigned int GetDepthTexID();

	void Cleanup(RaindropRenderer* rndr);
private:
	vec3f m_dir;
	vec3f m_color;
	float m_brightness;

	unsigned int m_depthMapTEX;
	unsigned int m_depthMapFBO;
	unsigned int m_shadowQuality;

	glm::mat4 m_lspace;
};

#endif //_DIR_LIGHT_H__