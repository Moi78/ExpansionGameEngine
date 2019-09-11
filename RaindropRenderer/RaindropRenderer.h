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

	void initWindow(int w, int h, std::string name);
	void ClearWindow(vec3f refreshColor);
	void SwapWindow();
	GLFWwindow* GetGLFWwindow();
	bool WantToClose();

	int getWindowHeigh();
	int getWindowWidth();

	RD_ShaderLoader* GetShader();
	RD_ShaderLoader* GetLightShader();

	void SetAmbientStrength(float strength);
	void SetAmbientColor(vec3f nColor);

	int AppendLight(RD_PointLight* ptLight);

	void SwitchShader(RD_ShaderLoader*);

	void RenderDbg();
private:
	void UpdateAmbientLighting();
	void UpdatePointsLighting();

	GLFWwindow* win;

	int m_height;
	int m_width;

	RD_ShaderLoader* m_shader;
	RD_ShaderLoader* m_LightShader;

	float ambientStrength;
	vec3f ambientColor;

	RD_FrameLimiter* m_frmLmt;

	std::vector<RD_PointLight*> m_pt_lights;

	RD_Mesh* m_DBG_light_mdl;
};

#endif // !_RAINDROP_RENDERER_H__