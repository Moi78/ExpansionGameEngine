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

#include <BD_StructMan.h>

#include <string>
#include <vector>

#include "RD_Callback.h"

class RAINDROPRENDERER_API RD_GUI
{
public:
	RD_GUI(RD_Callback*);
	~RD_GUI();

	void RenderTime();

private:
	RD_Callback* m_gui_cbck;
};

#endif //_RD_GUI_H__