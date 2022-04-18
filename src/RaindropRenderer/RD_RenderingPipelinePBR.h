#pragma once
#ifndef _RD_RENDERING_PIPELINE_PBR_H__
#define _RD_RENDERING_PIPELINE_PBR_H__

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RD_API __declspec(dllexport)
	#else
		#define RD_API __declspec(dllimport)
	#endif
#endif //_WIN32

#include "RD_RenderingPipeline.h"
#include "RD_RenderingAPI.h"
#include "RD_Texture.h"
#include "RD_Quad.h"
#include "RD_DirLight.h"

class RD_API RD_RenderingPipelinePBR : public RD_RenderingPipeline
{
public:
	RD_RenderingPipelinePBR();
	virtual ~RD_RenderingPipelinePBR();

	virtual void InitPipeline(RD_RenderingAPI* api, std::string engineDir);

	virtual void RenderScene(RD_RenderingAPI* api, RD_MaterialLibrary* sceneMeshData);
	virtual void RenderShadowMaps(RD_RenderingAPI* api, std::vector<RD_DirLight*> dlights, RD_MaterialLibrary* sceneData, vec3f camPos);

	virtual void ResizeFramebuffers(RD_RenderingAPI* api, const int w, const int h);

	virtual RD_ShaderStorageBuffer* GetShadowMapBufferHandle();

private:
	void CompileShaders(RD_RenderingAPI* api, std::string engineDir);
	void CreateGBuff(RD_RenderingAPI* api);
	void InitUBO(RD_RenderingAPI* api);

	void BlurShadow();

	RD_Quad* m_quad;

	//Shaders
	RD_ShaderLoader* m_bloom;
	RD_ShaderLoader* m_bloomApply;

	RD_ShaderLoader* m_light;
	RD_ShaderLoader* m_beauty;

	RD_ShaderLoader* m_shadow;
	RD_ShaderLoader* m_shadowBlur;
	RD_ShaderLoader* m_shadowCalc;

	RD_ShaderLoader* m_ssao;
	RD_ShaderLoader* m_ssaoBlur;

	RD_ShaderLoader* m_reflections;

	//Framebuffers
	Gbuff m_gbuff_struct;
	RD_FrameBuffer* m_gbuff;

	RD_FrameBuffer* m_light_fb;
	RD_FrameBuffer* m_shadow_fb;

	RD_FrameBuffer* m_shadow_blur_a;
	RD_FrameBuffer* m_shadow_blur_b;

	RD_FrameBuffer* m_bloom_a;
	RD_FrameBuffer* m_bloom_b;

	RD_FrameBuffer* m_final_passes;

	//UBO/SSBO
	RD_ShaderStorageBuffer* m_gbuff_handles;
	RD_ShaderStorageBuffer* m_shadowmaps;
	RD_ShaderStorageBuffer* m_sfx_handles;
	RD_ShaderStorageBuffer* m_final_passes_handle;

	RD_UniformBuffer* m_lspace_mats;
	RD_UniformBuffer* m_blurState;
};

struct GLSL_BlurState {
	float dir[3];
	int index;
	int threshold;
};

#endif //_RD_RENDERING_PIPELINE_PBR_H__