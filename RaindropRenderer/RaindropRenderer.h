#pragma once

#ifndef _RAINDROP_RENDERER_H__
#define _RAINDROP_RENDERER_H__

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#include "RD_ShaderLoader.h"

#include "vec3.h"

#include <glad/glad.h>
#include <glfw3.h>

#include <iostream>
#include <string>

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
	bool WantToClose();

	int getWindowHeigh();
	int getWindowWidth();

	RD_ShaderLoader* GetShader();

	void SetAmbientStrength(float strength);
	void SetAmbientColor(vec3f nColor);
private:
	void UpdateLighting();

	GLFWwindow* win;

	int m_height;
	int m_width;

	RD_ShaderLoader* m_shader;

	float ambientStrength;
	vec3f ambientColor;
};

#endif // !_RAINDROP_RENDERER_H__