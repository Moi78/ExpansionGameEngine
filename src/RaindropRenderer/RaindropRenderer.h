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

class RD_RenderingAPI;

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
	void SetFullscreenMode(const bool fullscr) const;
	void SetVSync(const bool vsync);
	bool IsVSyncActivated() const;

	bool DoNeedCamUpdate();
	
	std::string GetEngineDir() const;

	double GetLastDeltaTime() const;

	RD_GenericRessourceManager<RD_TextRenderer>* GetTxtRendererManager() const;

	//Rendering
	void RenderMeshes(RD_Camera* cam);
	void RenderShadowMeshes();

	void RenderParticles();
	void UpdateParticles();

	void RenderLightPass(const vec3f& camPos);
	void RenderLightsDepth(const vec3f& camPos);
	void RenderShadows();

	void RenderSSR();
	void RenderBloom();

	void RenderSSAO();
	void GenerateSSAOKernels();
	void GenerateSSAONoise();
	
	void RenderGbuff(RD_Camera*);

	void RenderPostProcess();
	void RenderBeauty();

	void ResizeViewport(vec2f pos, vec2f size);
	void DisableResizeOverride();
	bool GetResizeOverrideState() const;
	vec2f GetViewportSize() const;
	vec2f GetViewportPos() const;

	RD_Texture* GetBlankTexture() const;
	
	void AddToTextureGarbageCollector(unsigned int texID);
	void EmptyTextureGarbageCollector();

	void AddToFramebufferGarbageCollector(unsigned int fboID);
	void EmptyFramebufferGarbageCollector();

	void PushModelMatrix(mat4f& model);
	void PushLightProjViewMatrices(mat4f& lview, mat4f& lproj);

	RD_UniformBuffer* GetCameraMatrixBuffer();
	RD_UniformBuffer* GetCameraPosBuffer();

	RD_ShaderStorageBuffer* GetGlyphTexHandle();
	RD_UniformBuffer* GetTextColorUniform();

	//Lighting
	void SetAmbientStrength(float strength);
	void SetAmbientColor(const vec3f& nColor);
	float GetAmbientStrength();
	vec3f GetAmbientColor();

	void UpdatePointsLighting();
	void UpdateAmbientLighting();
	void UpdateDirLighting(const bool lspace_only = false);

	RD_ShaderStorageBuffer* GetShadowMapsBufferHandle();
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
	void SwitchShader(RD_ShaderLoader*);
	RD_ShaderLoader* GetShadowShader() const;
	RD_ShaderLoader* GetCurrentShader() const;
	RD_ShaderMaterial* FetchShaderFromFile(const std::string& ref, const std::string& texPathPrefix = "", const bool noreg = false);
	RD_MaterialLibrary* GetMaterialLibrary() const;
	int GetCurrentShaderStorageIndex();
	void IncrementCurrentShaderStorageIndex();

	void MakeEnvCubemapFromTexs(std::array<std::string, 6> texs);

	//Debug
	void RenderDbg(RD_Camera*);
	float GetFramerate() const;

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

	bool CreateGbuff();
	bool CreateGbuff_PBR();
	void DeleteGbuff();

	void SetErrorFlag(bool val);

	std::unique_ptr<RD_RenderingAPI> m_api;

	Pipeline m_pipeline;
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

	//Deffered Rendering
	Gbuff m_g_buffer;
	RD_FrameBuffer* m_gbuffer;
	RD_FrameBuffer* m_light_pprocess;
	RD_FrameBuffer* m_shadows_buffer;
	RD_FrameBuffer* m_shadows_blur;
	RD_FrameBuffer* m_shadows_blur_b;

	//PBR-Only stuff
	RD_FrameBuffer* m_ssao_buffer;
	RD_FrameBuffer* m_bloom_buffera;
	RD_FrameBuffer* m_bloom_bufferb;
	RD_FrameBuffer* m_reflections_buffer;

	//Internals shaders (some aren't used and compiled if Pipeline is not PBR)
	RD_ShaderLoader* m_shadowShader;

	RD_ShaderLoader* m_light_shader;
	RD_ShaderLoader* m_beauty_shader;

	RD_ShaderLoader* m_shadowCalc;
	RD_ShaderLoader* m_shadowBlur;

	RD_ShaderLoader* m_bloom; //PBR
	RD_ShaderLoader* m_bloom_apply; //PBR
	RD_ShaderLoader* m_ssr_shader; //PBR
	
	RD_ShaderLoader* m_ssao_shader; //PBR
	RD_ShaderLoader* m_ssao_blur_shader; //PBR

	RD_ShaderMaterial* m_dbgMat;

	std::vector<vec3f> m_ssao_kernels;
	RD_Texture* m_ssao_noise_tex;

	std::vector<RD_PostProcessEffect*> m_pp_effects;

	RD_ShaderLoader* m_CurrentShader;

	//Uniform buffers
	RD_UniformBuffer* m_dirLights_u;
	RD_UniformBuffer* m_pointLight_u;
	RD_UniformBuffer* m_ambient_u;
	RD_UniformBuffer* m_ssao_u;
	RD_UniformBuffer* m_model_u;
	RD_UniformBuffer* m_lightview_u;
	RD_UniformBuffer* m_lightspace_u;
	RD_UniformBuffer* m_lightcount_u;
	RD_UniformBuffer* m_text_color_u;
	RD_UniformBuffer* m_camera_matrix_u;
	RD_UniformBuffer* m_camera_location_u;

	//ShaderStorage
	RD_ShaderStorageBuffer* m_gbuff_tex_handles_s;
	RD_ShaderStorageBuffer* m_sfx_tex_handles_s;
	RD_ShaderStorageBuffer* m_blur_state_s;
	RD_ShaderStorageBuffer* m_ssao_tex_handle_s;
	RD_ShaderStorageBuffer* m_final_passes_tex_handle_s;
	RD_ShaderStorageBuffer* m_shadowmaps_s;
	RD_ShaderStorageBuffer* m_glyph_s;
	RD_ShaderStorageBuffer* m_final_pass_selector_s;

	RD_Cubemap* m_env_cmap;

	int m_current_shader_storage_index;
	int m_current_selector;

	vec2f m_vp_size, m_vp_pos;

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

#endif // !_RAINDROP_RENDERER_H__
