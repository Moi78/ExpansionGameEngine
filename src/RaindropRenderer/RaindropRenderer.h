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
#include "RD_Texture.h"
#include "RD_FrameBuffer.h"
#include "RD_Structs.h"

#include "RD_GenericRessourceManager.h"

#include <BD_MatRW.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <thread>
#include <mutex>
#include <stddef.h>

#include <vec3.h>
#include <vec2.h>

#ifdef _WIN32
	#include <Windows.h>
#endif

//Frwd Declaration
class RD_DirLight;
class RD_Mesh;
class RD_Quad;
class RD_ShaderMaterial;
class RD_Camera;
class RD_MaterialLibrary;
class RD_GUI_Manager;
class RD_PostProcessEffect;
class RD_TextRenderer;
class RD_ParticleEmitter;
class RD_Cubemap;

class RD_RenderingPipeline;
class RD_RenderingAPI;

class RAINDROPRENDERER_API RaindropRenderer {
public:
	RaindropRenderer(int w, int h, std::string windowName, API api, RD_RenderingPipeline* pline, int maxFramerate = 60, bool minInit = false, std::string EngineDir = "Engine");
	~RaindropRenderer();

	/*void initWindow(int w, int h, std::string name);*/
	void ClearWindow(vec3f refreshColor) const;
	void SwapWindow();
	bool WantToClose() const;
	RD_RenderingAPI* GetRenderingAPI() const;
	RD_RenderingPipeline* GetRenderingPipeline() const;

	int GetWindowHeigh() const;
	int GetWindowWidth() const;
	void SetFullscreenMode(const bool fullscr) const;
	void SetVSync(const bool vsync);
	bool IsVSyncActivated() const;

	bool DoNeedCamUpdate();

	void IncrementCurrentSSBOIdx();
	int GetCurrentSSBOIdx();

	std::string GetEngineDir() const;

	double GetLastDeltaTime() const;

	RD_GenericRessourceManager<RD_TextRenderer>* GetTxtRendererManager() const;
	RD_MaterialLibrary* GetMaterialLibrary();

	//Rendering
	void RenderParticles();
	void UpdateParticles();

	void ResizeViewport(vec2f pos, vec2f size);

	void DisableResizeOverride();
	bool GetResizeOverrideState() const;

	vec2f GetViewportSize() const;
	vec2f GetViewportPos() const;
	vec2f GetViewportScale() const;

	void SetQuadzone(vec2f pos, vec2f scale);

	RD_Texture* GetBlankTexture() const;
	RD_UniformBuffer* GetTextColorUniform();

	void RenderShadows(vec3f camPos);
	void RenderScene();
	void RenderDebug();

	void PushViewMatrix(mat4f view);
	void PushProjMatrix(mat4f proj);
	void PushModelMatrix(mat4f mdl);
	void PushCamPos(vec3f pos);

	//Lighting
	void SetAmbientStrength(float strength);
	void SetAmbientColor(const vec3f& nColor);
	float GetAmbientStrength();
	vec3f GetAmbientColor();

	void UpdatePointsLighting();
	void UpdateAmbientLighting();
	void UpdateDirLighting(const bool lspace_only = false);

	void PushLightProjView(mat4f lightproj, mat4f lightview);

	int GetDirLightsCount();

	//Elements registrations
	int AppendLight(RD_PointLight* ptLight);
	int AppendDirLight(RD_DirLight* dirLight);
	void RegisterMesh(RD_Mesh* mesh);
	void AddPostProcessEffect(RD_PostProcessEffect* effect);
	void RegisterParticleEmitter(RD_ParticleEmitter* emitter);

	//Elements unregistrations
	void UnregisterMesh(RD_Mesh*);
	void UnregisterPointLight(RD_PointLight*);
	void UnregisterDirLight(RD_DirLight*);
	void RemovePostProcessEffect(RD_PostProcessEffect* effect);
	void UnregisterParticleEmitter(RD_ParticleEmitter* emitter);

	void UnregisterAllMeshes();
	void UnregisterAllPointLights();
	void UnregisterAllDirLights();
	void UnregisterAllParticleEmitters();

	//Shading
	RD_ShaderMaterial* FetchShaderFromFile(const std::string& ref, const std::string& texPathPrefix = "", const bool noreg = false);

	void MakeEnvCubemapFromTexs(std::array<std::string, 6> texs);

	//Renderer Feature
	void EnableFeature(RendererFeature ftr);
	void DisableFeature(RendererFeature ftr);
	bool IsFeatureEnabled(RendererFeature ftr);
	void SendFeatureToShader(RD_ShaderLoader* shader, RendererFeature feature);

	int GetFrameLimit() const;
	bool GetErrorFlag() const;
private:

	void FillFeaturesArray();
	void EnableAllFeatures();

	void SetErrorFlag(bool val);

	std::unique_ptr<RD_RenderingAPI> m_api;

	RD_RenderingPipeline* m_pipeline;
	std::string m_engineDir;

	bool m_error_flag;
	bool m_resize_override;
	bool m_vsync;
	bool m_need_cam_updt;

	std::array<std::pair<std::string, bool>, 5> m_renderer_feature;

	float ambientStrength;
	vec3f ambientColor;

	std::unique_ptr<RD_FrameLimiter> m_frmLmt;

	std::vector<RD_PointLight*> m_pt_lights;
	std::vector<RD_DirLight*> m_DirLights;
	std::vector<RD_Mesh*> m_meshes;
	std::vector<RD_ParticleEmitter*> m_partEmitters;

	std::unique_ptr<RD_Mesh> m_DBG_light_mdl;
	std::unique_ptr<RD_Mesh> m_DBG_sound_emitter_mdl;

	RD_Texture* m_defTex;
	RD_Texture* m_blankTexture;

	std::vector<unsigned int> m_textureGarbageCollector;
	std::vector<unsigned int> m_framebufferGarbageCollector;

	std::unique_ptr<RD_Quad> m_quad;

	RD_UniformBuffer* m_model_u;
	RD_UniformBuffer* m_lightview_u;
	RD_UniformBuffer* m_lightspace_u;
	RD_UniformBuffer* m_lightcount_u;
	RD_UniformBuffer* m_text_color_u;
	RD_UniformBuffer* m_camera_matrix_u;
	RD_UniformBuffer* m_camera_location_u;
	RD_UniformBuffer* m_ambient_u;
	RD_UniformBuffer* m_dirLightData_u;
	RD_UniformBuffer* m_pointLightData_u;
	RD_UniformBuffer* m_quadzone_u;

	RD_Cubemap* m_env_cmap;

	int m_current_shader_storage_index;
	int m_current_selector;

	vec2f m_vp_size, m_vp_pos, m_vp_scale;

	//Ressource management
	std::unique_ptr<RD_MaterialLibrary> m_matlib;
	std::unique_ptr<RD_GenericRessourceManager<RD_TextRenderer>> m_txtRndrs;
};

template<class T>
int GetElemIndex(std::vector<T> array, T element) {
	auto it = std::find(array.begin(), array.end(), element);

	if (it != array.end()) {
		return std::distance(array.begin(), it);
	}
	else {
		std::cerr << "ERROR: (GetElemIndex) Element cannot be found !" << std::endl;
		return -1;
	}
}

struct ShaderBlurState {
	float dir[3];
	int index;
	int threshold = 0;
	int first_pass = 0;
};

struct GLSL_QuadZone {
	float pos[2];
	float scale[2];
};

#endif // !_RAINDROP_RENDERER_H__
