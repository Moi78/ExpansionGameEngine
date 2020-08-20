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
#include <RD_SizedQuad.h>
#include <RD_GUI_Element.h>
#include <RD_GUI_Manager.h>

#include <glm/glm.hpp>

#include <vec3.h>

#include "EXP_Game.h"

#include <memory>

class EXPGE_API EXP_GUI_ColorCache : public RD_GUI_Element
{
public:
	EXP_GUI_ColorCache(EXP_Game* game, vec3f color, float opacity, float sizex, float sizey, float posx, float posy);
	~EXP_GUI_ColorCache();

	virtual void RenderElement() override;

private:
	EXP_Game* m_game;

	std::unique_ptr<RD_Quad> m_surface;

	glm::mat4 m_mdl;
	glm::mat4 m_proj;

	vec3f m_color;
	float m_opacity;
};

