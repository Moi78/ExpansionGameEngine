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

#include "EXP_Game.h"

#include "glad/glad.h"

#include <RaindropRenderer.h>
#include <RD_Quad.h>
#include <RD_GUI_Manager.h>
#include <RD_GUI_Element.h>
#include <RD_Texture.h>

#include <memory>

#include <mat4.h>
#include <vec3.h>

class EXPGE_API EXP_GUI_ImageTexture : public RD_GUI_Element
{
public:
	EXP_GUI_ImageTexture(EXP_Game* game, std::string texRef, float opacity, float sizex, float sizey, float posx, float posy);
	~EXP_GUI_ImageTexture();

	virtual void RenderElement() override;
	virtual void RebuildElement() override;

	void SetPosition(vec3f nPos);
	vec3f GetPosition();

private:
	std::unique_ptr<RD_Quad> m_surface;
	RD_ShaderLoader* m_gui_shader;

	RD_Texture* m_image;

	float m_opacity;

	EXP_Game* m_game;

	vec3f m_pos;
	vec3f m_size;

	mat4f m_mdl;
	mat4f m_proj;
};

