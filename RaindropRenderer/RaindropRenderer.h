#pragma once

#ifndef _RAINDROP_RENDERER_H__
#define _RAINDROP_RENDERER_H__

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#include "RD_ShaderLoader.h"
#include "RD_FrameLimiter.h"
#include "RD_Mesh.h"
#include "RD_PointLight.h"
#include "RD_DirLight.h"

#include <BD_StructMan.h>

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <string>
#include <vector>

#include <vec3.h>

#include <Windows.h>

class RAINDROPRENDERER_API RaindropRenderer {
public:
	RaindropRenderer(int w, int h, std::string windowName);
	~RaindropRenderer();

	void initGlad();
	void MinInit();

	void initWindow(int w, int h, std::string name);
	void ClearWindow(vec3f refreshColor);
	void SwapWindow();
	GLFWwindow* GetGLFWwindow();
	bool WantToClose();

	int getWindowHeigh();
	int getWindowWidth();

	RD_ShaderLoader* GetShader();
	RD_ShaderLoader* GetLightShader();

	//Lighting
	void SetAmbientStrength(float strength);
	void SetAmbientColor(vec3f nColor);

	int AppendLight(RD_PointLight* ptLight);
	int AppendDirLight(RD_DirLight* dirLight);

	//Shading
	void SwitchShader(RD_ShaderLoader*);
	RD_ShaderLoader* DBG_GetLightShader();
	RD_ShaderLoader* DBG_GetGameViewShader();
	RD_ShaderLoader* GetCurrentShader();

	//Debug
	void RenderDbg();
	float GetFramerate();

	//Renderer Feature
	void EnableFeature(RendererFeature ftr);
	void DisableFeature(RendererFeature ftr);
	bool IsFeatureEnabled(RendererFeature ftr);

	void UpdatePointsLighting();
private:
	void UpdateAmbientLighting();
	void UpdateDirLighting();

	void FillPtLightIndice(int index);
	void FillDirLightIndice(int index);

	void FillFeaturesStringArray();
	void FillFeatureStateArray();
	void EnableAllFeatures();

	//void glfwWinCallback(GLFWwindow* win, int w, int h);

	GLFWwindow* win;

	int m_height;
	int m_width;

	RD_ShaderLoader* m_shader;
	RD_ShaderLoader* m_LightShader;

	RD_ShaderLoader* m_CurrentShader;
	bool m_gview_shader_in_use;

	std::string m_features_string[3];
	bool m_features_state[3];

	float ambientStrength;
	vec3f ambientColor;

	RD_FrameLimiter* m_frmLmt;

	std::vector<RD_PointLight*> m_pt_lights;
	std::vector<RD_DirLight*> m_DirLights;

	RD_Mesh* m_DBG_light_mdl;
};

void glfwWinCallback(GLFWwindow* win, int w, int h);

#endif // !_RAINDROP_RENDERER_H__