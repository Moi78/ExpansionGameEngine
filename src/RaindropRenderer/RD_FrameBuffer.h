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

#include "RaindropRenderer.h"
#include "RD_Texture.h"

class RAINDROP_RENDERER_API RD_FrameBuffer
{
public:
	RD_FrameBuffer(int w, int h, unsigned int nbrAttachement);
	~RD_FrameBuffer();

	unsigned int GetFBO();

	void BindFBO();
	void UnbindFBO();

	int GetNumberOfAttachements();
	RD_Texture* GetAttachementByIndex(int index);

private:
	void CreateFBO();
	void CreateAttachements();

	std::vector<RD_Texture*> m_attachments;

	unsigned int m_FBO, m_nbrAttachement;

	int m_w, m_h;
};

#endif //_RD_FRAME_BUFFER_H__