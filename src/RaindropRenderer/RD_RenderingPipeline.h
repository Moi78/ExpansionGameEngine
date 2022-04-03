#pragma once
#ifndef _RD_RENDERING_PIPELINE_H__
#define _RD_RENDERING_PIPELINE_H__

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RD_API __declspec(dllexport)
	#else
		#define RD_API __declspec(dllimport)
	#endif
#endif //_WIN32

#include "RD_MaterialLibrary.h"
#include "RD_RenderingAPI.h"

#include <string>

class RD_API RD_RenderingPipeline {
public:
	virtual ~RD_RenderingPipeline() {};

	virtual void InitPipeline(RD_RenderingAPI* api, std::string engineDir) = 0;

	virtual void RenderScene(RD_RenderingAPI* api, RD_MaterialLibrary* sceneMeshData) = 0;
	virtual void RenderShadowMaps(RD_RenderingAPI* api, std::vector<RD_DirLight*> dlights, RD_MaterialLibrary* sceneData, vec3f camPos) = 0;

	virtual RD_ShaderStorageBuffer* GetShadowMapBufferHandle() = 0;
};

#endif //__RD_RENDERING_PIPELINE_H__