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

#include "EXP_Game.h"
#include "EXP_Component.h"

#include <RaindropRenderer.h>
#include <RD_TextRenderer.h>
#include <RD_ShaderLoader.h>
#include <RD_Mesh.h>

#include <string>
#include <vector>

class EXPGE_API EXP_TextSurface :
	private RD_Mesh,
	public EXP_Component
{
public:
	EXP_TextSurface(EXP_Game* game,
		RD_ShaderMaterial* mat,
		const std::string& text,
		const int size,
		const std::string& font,
		vec3f pos, vec3f rot, vec3f scale,
		vec3f color = vec3f()
	);

	~EXP_TextSurface();

	virtual void render(RD_Camera* cam);

	virtual void SetPosition(vec3f nPos);
	virtual void SetRotation(vec3f nRot);
	virtual void SetScale(vec3f nScale);

	virtual void UseParentMatrix(mat4f parent);
private:
	void UpdateMatrices();

	EXP_Game* m_game;

	RD_ShaderLoader* m_shader;
	RD_TextRenderer* m_txtRndr;

	std::string m_txt;
	vec3f m_color;

	std::vector<mat4f> m_letters_prop;
	vec3f m_space;
	vec3f m_letter_spacing;
};

