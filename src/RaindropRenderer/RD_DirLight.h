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
#include "RD_MaterialLibrary.h"
#include "RD_RenderingPipelinePBR.h"

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

	void DepthRender(RD_RenderingAPI* rndr, RD_ShaderLoader* shadowShader, RD_MaterialLibrary* sceneData, vec3f CamPos);
	void SetUpShadowFB(RaindropRenderer* rndr, unsigned int shadowQual);
	mat4f GetLightSpace() const;
	RD_Texture* GetDepthTexID() const;

	void SendOnBuffer(RD_UniformBuffer* ubo, int idx, int offset);

	void Cleanup(RaindropRenderer* rndr);

	void SetNearFarPlanes(float nearp, float farp);
private:
	RaindropRenderer* m_rndr;

	vec3f m_dir;
	vec3f m_color;
	float m_brightness;

	float m_far_plane, m_near_plane;

	RD_FrameBuffer* m_fbo;

	unsigned int m_shadowQuality;
	bool m_shadowCaster;

	mat4f m_lspace;
};

struct GLSLDirLight {
	float dir[3];
	float __pad;
	float color[3];
	float brightness;
};

#endif //_DIR_LIGHT_H__