#pragma once

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif // EXPANSIONGAMEENGINECORE_EXPORTS
#else
	#define EXPGE_API
#endif //_WIN32

#include <RaindropRenderer.h>
#include <RD_Quad.h>
#include <RD_GUI_Element.h>
#include <RD_GUI_Manager.h>
#include <RD_TextRenderer.h>
#include <RD_Texture.h>

#include "EXP_Game.h"

#include <memory>

#include <vec3.h>
#include <vec2.h>
#include <mat4.h>

class EXPGE_API EXP_GUI_Text : public RD_GUI_Element
{
public:
	EXP_GUI_Text(EXP_Game* game, const std::string& fontRef, const std::string& text, const int fontSize, vec2f pos, vec2f scale);
	virtual ~EXP_GUI_Text();

	virtual void RenderElement() override;
	virtual void RebuildElement() override;

	void SetPosition(vec2f nPos);
	vec2f GetPosition();
private:
	std::unique_ptr<RD_Quad> m_surface;
	RD_ShaderLoader* m_shader;

	RD_TextRenderer* m_txtrndr;
	std::string m_text;

	EXP_Game* m_game;

	vec3f m_pos;
	vec3f m_size;

	mat4f m_mdl;
	mat4f m_proj;
};

