#pragma once

#ifndef _RAINDROP_RENDERER_H__
#define _RAINDROP_RENDERER_H__

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#else

#define RAINDROPRENDERER_API

#endif //_WIN32

#ifdef _DEBUG
	#define RENDER_DEBUG_ENABLED true
#else
	#define RENDER_DEBUG_ENABLED false
#endif //_DEBUG

#include "RD_ShaderLoader.h"
#include "RD_FrameLimiter.h"
#include "RD_PointLight.h"
#include "RD_Materials.h"
#include "RD_Texture.h"
#include "RD_FrameBuffer.h"
#include "RD_Structs.h"

#include <BD_MatRW.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>

#include <vec3.h>
#include <vec2.h>

#ifdef _WIN32
	#include <Windows.h>
#endif

//Frwd Declaration
class RD_DirLight;
class RD_Mesh;
class RD_Quad;
//class RD_FrameBuffer;
class RD_Camera;
class RD_MaterialLibrary;
class RD_GUI_Manager;
class RD_PostProcessEffect;

class RD_RenderingAPI;

enum API {
	OPENGL,
	VULKAN,
	DIRECTX
};

enum Pipeline {
	PBR_ENGINE,
	LAMBERT_ENGINE
};

class RAINDROPRENDERER_API RaindropRenderer {
public:
	RaindropRenderer(int w, int h, std::string windowName, API api, Pipeline pline = Pipeline::LAMBERT_ENGINE, int maxFramerate = 60, bool minInit = false, std::string EngineDir = "Engine");
	~RaindropRenderer();

	/*void initWindow(int w, int h, std::string name);*/
	void ClearWindow(vec3f refreshColor) const;
	void SwapWindow();
	bool WantToClose() const;
	RD_RenderingAPI* GetRenderingAPI() const;

	int getWindowHeigh() const;
	int getWindowWidth() const;
	void SetFullscreenMode(const bool fullscr);
	
	std::string GetEngineDir();

	double GetLastDeltaTime();

	//Rendering
	void RenderMeshes(RD_Camera* cam);
	void RenderShadowMeshes();

	void RenderLightPass(vec3f camPos);
	void RenderLightsDepth(vec3f camPos);
	void RenderShadows();

	void RenderSSR(RD_Camera* cam);

	void RenderSSAO(RD_Camera* cam);
	void GenerateSSAOKernels();
	void GenerateSSAONoise();
	
	void RenderGbuff(RD_Camera*);

	void RenderPostProcess();
	void RenderBeauty();

	void RecreateGbuff();

	void ResizeViewport(vec2f pos, vec2f size);
	void DisableResizeOverride();
	bool GetResizeOverrideState() const;
	vec2f GetViewportSize() const;
	vec2f GetViewportPos() const;

	RD_Texture* GetBlankTexture();
	
	void AddToTextureGarbageCollector(unsigned int texID);
	void EmptyTextureGarbageCollector();

	void AddToFramebufferGarbageCollector(unsigned int fboID);
	void EmptyFramebufferGarbageCollector();

	//Lighting
	void SetAmbientStrength(float strength);
	void SetAmbientColor(vec3f nColor);

	//Elements registrations
	int AppendLight(RD_PointLight* ptLight);
	int AppendDirLight(RD_DirLight* dirLight);
	void RegisterMesh(RD_Mesh* mesh);
	void AddPostProcessEffect(RD_PostProcessEffect* effect);

	//Elements unregistrations
	void UnregisterMesh(RD_Mesh*);
	void UnregisterPointLight(RD_PointLight*);
	void UnregisterDirLight(RD_DirLight*);
	void RemovePostProcessEffect(RD_PostProcessEffect* effect);

	void UnregisterAllMeshes();
	void UnregisterAllPointLights();
	void UnregisterAllDirLights();

	//Shading
	void SwitchShader(RD_ShaderLoader*);
	RD_ShaderLoader* GetShadowShader() const;
	RD_ShaderLoader* GetCurrentShader();
	RD_ShaderMaterial* FetchShaderFromFile(const std::string& ref);
	RD_MaterialLibrary* GetMaterialLibrary();

	//Debug
	void RenderDbg(RD_Camera*);
	float GetFramerate();

	//Renderer Feature
	void EnableFeature(RendererFeature ftr);
	void DisableFeature(RendererFeature ftr);
	bool IsFeatureEnabled(RendererFeature ftr);
	void SendFeaturesToShader(RD_ShaderLoader* shader);

	void UpdatePointsLighting();

	//GUI
	void RenderGUI_Screen();
	RD_GUI_Manager* GetGUI_Manager();

	int GetFrameLimit();
	bool GetErrorFlag();
private:
	void UpdateAmbientLighting();
	void UpdateDirLighting();

	void FillPtLightIndice(int index);
	void FillDirLightIndice(int index);

	void FillFeaturesStringArray();
	void FillFeatureStateArray();
	void EnableAllFeatures();

	bool CreateGbuff();
	bool CreateGbuff_PBR();
	void DeleteGbuff();

	void SetErrorFlag(bool val);

	std::unique_ptr<RD_RenderingAPI> m_api;

	Pipeline m_pipeline;
	std::string m_engineDir;

	bool m_error_flag;
	bool m_resize_override;

	std::string m_features_string[3];
	bool m_features_state[3];

	float ambientStrength;
	vec3f ambientColor;

	std::unique_ptr<RD_FrameLimiter> m_frmLmt;

	std::vector<RD_PointLight*> m_pt_lights;
	std::vector<RD_DirLight*> m_DirLights;
	std::vector<RD_Mesh*> m_meshes;

	std::unique_ptr<RD_Mesh> m_DBG_light_mdl;
	std::unique_ptr<RD_Mesh> m_DBG_sound_emitter_mdl;

	RD_Texture* m_defTex;
	RD_Texture* m_blankTexture;

	std::vector<unsigned int> m_textureGarbageCollector;
	std::vector<unsigned int> m_framebufferGarbageCollector;

	//Deffered Rendering
	Gbuff m_g_buffer;
	RD_FrameBuffer* m_gbuffer;
	RD_FrameBuffer* m_light_pprocess;
	RD_FrameBuffer* m_shadows_buffer;

	//PBR-Only stuff
	RD_FrameBuffer* m_ssao_buffer;

	std::unique_ptr<RD_Quad> m_quad;

	std::unique_ptr<RD_GUI_Manager> m_gui_manager;

	//Internals shaders (some are'nt use and compiled if Pipeline is not PBR)
	RD_ShaderLoader* m_shadowShader;

	RD_ShaderLoader* m_light_shader;
	RD_ShaderLoader* m_beauty_shader;
	RD_ShaderLoader* m_shadowCalc;
	
	RD_ShaderLoader* m_ssr_shader;
	
	RD_ShaderLoader* m_ssao_shader;
	RD_ShaderLoader* m_ssao_blur_shader;

	RD_ShaderMaterial* m_dbgMat;

	std::vector<vec3f> m_ssao_kernels;
	RD_Texture* m_ssao_noise_tex;

	std::vector<RD_PostProcessEffect*> m_pp_effects;

	RD_ShaderLoader* m_CurrentShader;

	vec2f m_vp_size, m_vp_pos;

	//Materials
	std::unique_ptr<RD_MaterialLibrary> m_matlib;
};

template<class T>
int GetElemIndex(std::vector<T> array, T element) {
	auto it = std::find(array.begin(), array.end(), element);

	if (it != array.end()) {
		return std::distance(array.begin(), it);
	}
	else {
		std::cerr << "ERROR: Element cannot be found !" << std::endl;
		return -1;
	}
}

#endif // !_RAINDROP_RENDERER_H__
