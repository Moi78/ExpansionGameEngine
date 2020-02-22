#pragma once

#ifndef _RD_GUI_H__
#define _RD_GUI_H__

#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif

#include <BD_StructMan.h>

#include <string>
#include <vector>

#include "imgui.h"

class RAINDROPRENDERER_API RD_GUI
{
public:
	RD_GUI(std::string WindowName);
	~RD_GUI();

	void RenderTime();

private:
	std::vector<BD_GUI_Element> m_gui;

	std::vector<std::string> m_buttonProperties;

	std::string m_windowName;
};

#endif //_RD_GUI_H__