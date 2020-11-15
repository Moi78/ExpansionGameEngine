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

#define DEPTH_ATTACHEMENT 0
#define DEPTH24_STENCIL8_ATTACHEMENT 1

#define DEPTH_COMPONENT 0
#define DEPTH_STENCIL_COMPONENT 1

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>

#include "RD_Texture.h"

struct Attachement {
	RD_Texture* tex;
	unsigned int format;
};

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

	virtual void ConfigureRenderbuffer(int storage, int attachement) = 0;

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

	virtual void ConfigureRenderbuffer(int storage, int attachement);

	virtual void ChangeFramebufferSize(int nw, int nh);

private:
	virtual void CreateFBO();

	std::vector<Attachement> m_attachments;

	unsigned int m_FBO, m_RBO, m_nbrAttachement;

	int m_w, m_h;
	int m_storage, m_rbo_attachement;
};

#endif //BUILD_OPENGL

#endif //_RD_FRAME_BUFFER_H__