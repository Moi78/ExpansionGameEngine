#include "pch.h"
#include "EXP_TextSurface.h"

EXP_TextSurface::EXP_TextSurface(
	EXP_Game* game,
	RD_ShaderMaterial* mat,
	const std::string& text,
	const int size,
	const std::string& font,
	vec3f pos, vec3f rot, vec3f scale) :

	EXP_Component(pos, rot, scale),
	RD_Mesh(game->GetRenderer(), mat, pos, rot, scale)
{
	m_game = game;
	m_shader = mat->GetShader();

	m_txtRndr = new RD_TextRenderer(game->GetRenderer());
	m_txtRndr->LoadFont(game->GetFilePathByRef(font), size);

	m_txt = text;

	loadMesh(game->GetGameInfo().RootEngineContentFolder + "/Meshes/plane.msh");
	SetShadowCasting(false);

	game->RegisterMesh(reinterpret_cast<RD_Mesh*>(this));
}

EXP_TextSurface::~EXP_TextSurface() {
	delete m_txtRndr;
}

void EXP_TextSurface::render(RD_Camera* cam) {
	m_shader->useShader();

	m_buffer->BindBuffer();

	//Retaining these values
	vec3f pos = EXP_Component::m_pos;
	vec3f scale = EXP_Component::m_scale;

	for (auto c : m_txt) {
		if (c == ' ') {						//In case of space
			addTranslation(vec3f(1.0f));
			continue;
		}

		vec2f met = m_txtRndr->GetGlyphRelativeMetrics(c);
		vec3f Nscale = scale * vec3f(met.getX() / met.getY(), 1.0f, 1.0f);
		SetScale(Nscale);

		m_shader->SetMatrix("model", m_mdl);

		m_txtRndr->GetGlyphTexture(c)->BindTexture(0);
		m_rndr->GetRenderingAPI()->Draw(m_buffer);

		//int adv = m_txtRndr->GetGlyphAdvance(c);
		addTranslation(vec3f(2.0f * (met.getX() / met.getY())));
	}

	SetPosition(pos);
	SetScale(scale);

	m_buffer->UnbindBuffer();
}

void EXP_TextSurface::SetPosition(vec3f nPos) {
	EXP_Component::m_pos = nPos;
	RD_Mesh::SetPosition(nPos);
}

void EXP_TextSurface::SetRotation(vec3f nRot) {
	EXP_Component::m_rot = nRot;
	RD_Mesh::SetRotation(nRot);
}

void EXP_TextSurface::SetScale(vec3f nScale) {
	EXP_Component::m_scale = nScale;
	RD_Mesh::SetScale(nScale);
}

void EXP_TextSurface::UseParentMatrix(mat4f parent) {
	RD_Mesh::SetParentMatrix(parent);
}