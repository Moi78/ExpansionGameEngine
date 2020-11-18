#pragma once

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif
#else

	#define EXPGE_API

#endif

#include <RaindropRenderer.h>
#include <RD_Quad.h>
#include <RD_GUI_Element.h>
#include <RD_GUI_Manager.h>
#include <RD_ShaderLoader.h>

#include <mat4.h>
#include <vec3.h>

#include "EXP_Game.h"

#include <memory>

class EXPGE_API EXP_GUI_ColorCache : public RD_GUI_Element
{
public:
	EXP_GUI_ColorCache(EXP_Game* game, vec3f color, float opacity, float sizex, float sizey, float posx, float posy);
	~EXP_GUI_ColorCache();

	virtual void RenderElement() override;
	virtual void RebuildElement() override;

	void SetPosition(vec3f nPos);
	vec3f GetPosition();

private:
	EXP_Game* m_game;

	std::unique_ptr<RD_Quad> m_surface;
	RD_ShaderLoader* m_gui_shader;

	mat4f m_mdl;
	mat4f m_proj;

	vec3f m_pos;
	vec3f m_size;

	vec3f m_color;
	float m_opacity;
};

