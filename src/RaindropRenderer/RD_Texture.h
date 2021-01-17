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

#define IMGFORMAT_R 1
#define IMGFORMAT_RG 2
#define IMGFORMAT_RGB 3
#define IMGFORMAT_RGBA 4

#define IMGFORMAT_R16F 5
#define IMGFORMAT_RG16F 6
#define IMGFORMAT_RGB16F 7
#define IMGFORMAT_RGBA16F 8

#define IMGFORMAT_DEPTH 9

#define SCALEMODE_NEAREST 0
#define SCALEMODE_LINEAR 1

#define WRAPMODE_REPEAT 0
#define WRAPMODE_CLAMP2EDGE 1

#include <string>
#include <vector>

#include <vec3.h>

class RAINDROPRENDERER_API RD_Texture
{
public:
	RD_Texture() {}
	virtual ~RD_Texture() {};

	virtual void LoadTexture(const std::string& tex, bool flipTex = true) = 0;
	virtual void GenerateColorTex(vec3f color) = 0;
	virtual void CreateTextureFromPixels(
		void* pixels,
		int w,
		int h,
		unsigned int format = IMGFORMAT_RGB,
		unsigned int wrapmode = WRAPMODE_REPEAT) = 0;

	virtual void CreateAndAttachToFramebuffer(
		int w, int h,
		unsigned int FBO,
		unsigned int attachement = 0,
		unsigned int format = IMGFORMAT_RGB,
		unsigned int scaleMode = SCALEMODE_LINEAR,
		unsigned int wrapmode = WRAPMODE_REPEAT) = 0;
	
	virtual void BindTexture(unsigned int tex_unit) = 0;

	virtual unsigned int GetTextureID() = 0;

	virtual void DeleteTexture() = 0;
};

#ifdef BUILD_OPENGL

#include <glad/glad.h>

class RAINDROPRENDERER_API RD_Texture_GL : public RD_Texture
{
public:
	RD_Texture_GL();
	virtual ~RD_Texture_GL();

	virtual void LoadTexture(const std::string& tex, bool flipTex = true);
	virtual void GenerateColorTex(vec3f color);
	virtual void CreateTextureFromPixels(void* pixels, int w, int h, unsigned format, unsigned int wrapmode) override;
	virtual void CreateAndAttachToFramebuffer(
		int w, int h,
		unsigned int FBO, 
		unsigned int attachement = 0,
		unsigned int format = IMGFORMAT_RGB,
		unsigned int scaleMode = SCALEMODE_LINEAR,
		unsigned int wrapmode = WRAPMODE_REPEAT);
	
	virtual void BindTexture(unsigned int tex_unit = 0);

	virtual void DeleteTexture();

	virtual unsigned int GetTextureID();

private:
	void GetGLformat(
		unsigned int format,
		unsigned int scaleMode,
		unsigned int wrapmode,
		unsigned int* formatgl,
		unsigned int* typeGL,
		unsigned int* scaleModegl,
		unsigned int* wrapmodeGL); //I KNOW THAT IS C-STYLE >:)
	
	unsigned int m_texture;
};

#endif //BUILD_OPENGL

#endif //_RD_TEXTURE_H__