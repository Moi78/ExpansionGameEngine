#pragma once

#ifndef _RD_TEXTURE_H__
#define _RD_TEXTURE_H__

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#else

#define RAINDROPRENDERER_API

#endif //_WIN32

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>

#include <vec3.h>

class RAINDROPRENDERER_API RD_Texture
{
public:
	RD_Texture();
	~RD_Texture();

	void LoadTexture(std::string filepath, bool flipTex = true);
	void GenerateColorTex(vec3f color);
	void CreateAndAttachToFramebuffer(int w, int h, unsigned int FBO, unsigned int attachement = GL_COLOR_ATTACHMENT0, unsigned int format = GL_RGB);
	void BindTexture(unsigned int tex_unit = GL_TEXTURE0);

	unsigned int GetTextureID();

private:
	unsigned int m_texture;
};

#endif //_RD_TEXTURE_H__