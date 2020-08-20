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

class EXPGE_API EXP_GUI_ImageTexture : public RD_GUI_Element
{
public:
	EXP_GUI_ImageTexture(EXP_Game* game, std::string texRef, float opacity, float posx, float posy, float sizex, float sizey);
	~EXP_GUI_ImageTexture();

	virtual void RenderElement() override;

private:
	std::unique_ptr<RD_Quad> m_surface;
	std::unique_ptr<RD_Texture> m_image;

	float m_opacity;

	EXP_Game* m_game;

	glm::mat4 m_mdl;
	glm::mat4 m_proj;
};

