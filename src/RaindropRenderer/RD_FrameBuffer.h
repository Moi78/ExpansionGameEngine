#pragma once

#ifndef _RD_FRAME_BUFFER_H__
#define _RD_FRAME_BUFFER_H__

#ifdef _WIN32

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROP_RENDERER_API __declspec(dllexport)
#else
#define RAINDROP_RENDERER_API __declspec(dllimport)
#endif //RAINDROPRENDERER_EXPORTS

#else

#define RAINDROP_RENDERER_API

#endif //_WIN32

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "RaindropRenderer.h"
#include "RD_Texture.h"

class RAINDROP_RENDERER_API RD_FrameBuffer
{
public:
	RD_FrameBuffer() {}
	virtual ~RD_FrameBuffer() {};

	virtual unsigned int GetFBO() = 0;

	virtual void BindFBO() = 0;
	virtual void UnbindFBO() = 0;

	virtual int GetNumberOfAttachements() = 0;
	virtual RD_Texture* GetAttachementByIndex(int index) = 0;

	virtual void AddAttachement(unsigned int format) = 0;
	virtual void BuildFBO() = 0;

	virtual void ChangeFramebufferSize(int nw, int nh) = 0;

private:
	virtual void CreateFBO() = 0;
};

#ifdef BUILD_OPENGL

class RAINDROP_RENDERER_API RD_FrameBuffer_GL : public RD_FrameBuffer
{
public:
	RD_FrameBuffer_GL(int w, int h);
	virtual ~RD_FrameBuffer_GL();

	virtual unsigned int GetFBO();

	virtual void BindFBO();
	virtual void UnbindFBO();

	virtual int GetNumberOfAttachements();
	virtual RD_Texture* GetAttachementByIndex(int index);

	virtual void AddAttachement(unsigned int format);
	virtual void BuildFBO();

	virtual void ChangeFramebufferSize(int nw, int nh);

private:
	virtual void CreateFBO();

	std::vector<RD_Texture*> m_attachments;

	unsigned int m_FBO, m_nbrAttachement;

	int m_w, m_h;
};

#endif //BUILD_OPENGL

#endif //_RD_FRAME_BUFFER_H__