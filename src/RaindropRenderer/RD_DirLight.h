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
#include "RD_FrameBuffer.h"
#include "RD_RenderingAPI.h"

class RAINDROPRENDERER_API RD_DirLight
{
public:
	RD_DirLight(RaindropRenderer* rndr, vec3f dir, vec3f color, float brightness);
	virtual ~RD_DirLight();

	void SetLightDir(vec3f nDir);
	vec3f GetLightDir();

	void SetLightColor(vec3f nColor);
	vec3f GetLightColor();

	void SetLightBrightness(float nBrightness);
	float GetBrightness();

	void SetShadowCasting(bool scast);
	bool GetShadowCasting() const;

	void DepthRender(RaindropRenderer*, vec3f CamPos);
	void SetUpShadowFB(RaindropRenderer* rndr, unsigned int shadowQual);
	mat4f GetLightSpace() const;
	RD_Texture* GetDepthTexID() const;

	void Cleanup(RaindropRenderer* rndr);

	void SetNearFarPlanes(float nearp, float farp);
private:
	vec3f m_dir;
	vec3f m_color;
	float m_brightness;

	unsigned int m_depthMapTEX;
	unsigned int m_depthMapFBO;

	float m_far_plane, m_near_plane;

	RD_FrameBuffer* m_fbo;

	unsigned int m_shadowQuality;

	bool m_shadowCaster;

	mat4f m_lspace;
};

#endif //_DIR_LIGHT_H__