#pragma once

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport
	#endif
#else
	#define EXPGE_API
#endif //_WIN32

#include <RaindropRenderer.h>
#include <RD_GUI_Element.h>
#include <RD_Quad.h>
#include <RD_Texture.h>
#include <RD_GUI_Manager.h>
#include <RD_Callback.h>

#include "EXP_Game.h"
#include "EXP_InputHandler.h"
#include "EXP_Callbacks.h"

#include <string>
#include <memory>

#include <vec3.h>
#include <mat4.h>

struct EXP_ButtonImages {
	std::string texRefNormal;
	std::string texRefHover;
	std::string texRefPressed;
};

class EXPGE_API EXP_GUI_Button : RD_GUI_Element
{
public:
	EXP_GUI_Button(EXP_Game* game, RD_Callback* func, EXP_ButtonImages images, float opacity, float sizex, float sizey, float posx, float posy, bool oneTimeBtn = false);
	~EXP_GUI_Button();

	virtual void RenderElement();
	virtual void RebuildElement();

	void SetPosition(vec3f nPos);
	vec3f GetPosition();

private:
	EXP_Game* m_game;

	vec3f m_pos;
	vec3f m_size;

	RD_Texture* m_normal;
	RD_Texture* m_hover;
	RD_Texture* m_pressed;

	std::unique_ptr<RD_Quad> m_surface;
	RD_ShaderLoader* m_gui_shader;

	RD_Callback* m_func;

	mat4f m_mdl;
	mat4f m_proj;

	float m_opacity;

	bool m_oneTimeButton;
	bool m_released;
};

