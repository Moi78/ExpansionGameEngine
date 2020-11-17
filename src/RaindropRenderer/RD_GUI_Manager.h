#pragma once

#ifndef _RD_GUI_H__
#define _RD_GUI_H__

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#else

#define RAINDROPRENDERER_API

#endif //_WIN32

#include "RaindropRenderer.h"
#include "RD_ShaderLoader.h"
#include "RD_Quad.h"
#include "RD_SizedQuad.h"
#include "RD_GUI_Element.h"
#include "RD_MaterialLibrary.h"
#include "RD_FrameBuffer.h"
#include "RD_RenderingAPI.h"

#include <string>
#include <vector>
#include <memory>

class RAINDROPRENDERER_API RD_GUI_Manager
{
public:
	RD_GUI_Manager(RaindropRenderer* rndr);
	~RD_GUI_Manager();

	void InitManager();

	void RenderScreen();
	RD_Texture* GetScreenTexture();

	void RebuildFramebuffer();

	void RegisterElement(RD_GUI_Element* elem);
	void UnregisterElement(RD_GUI_Element* elem);

	RD_MaterialLibrary* GetGUIshaderManager();

private:
	void CreateFramebuffer();
	void DeleteFrambuffer();

	RaindropRenderer* m_rndr;
	RD_FrameBuffer* m_framebuffer;

	std::unique_ptr<RD_MaterialLibrary> m_matLib;

	std::vector<RD_GUI_Element*> m_gui_elements;
};

class ScreenCleaner : public RD_GUI_Element {
public:
	ScreenCleaner(std::string workingDir);
	~ScreenCleaner();

	virtual void RenderElement() override;

private:
	std::unique_ptr<RD_Quad> m_surface;
};

#endif //_RD_GUI_H__