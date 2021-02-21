#include "pch.h"
#include "EXP_TextSurface.h"

EXP_TextSurface::EXP_TextSurface(
	EXP_Game* game,
	RD_ShaderMaterial* mat,
	const std::string& text,
	const int size,
	const std::string& font,
	vec3f pos, vec3f rot, vec3f scale,
	vec3f color) :

	EXP_Component(pos, rot, scale),
	RD_Mesh(game->GetRenderer(), mat, pos, rot, scale)
{
	m_game = game;
	m_shader = mat->GetShader();
	m_color = color;

	m_txtRndr = new RD_TextRenderer(game->GetRenderer());
	m_txtRndr->LoadFont(game->GetFilePathByRef(font), size);

	m_txt = text;

	loadMesh(game->GetGameInfo().RootEngineContentFolder + "/Meshes/plane.msh");
	SetShadowCasting(false);

	m_space = (vec3f(1.0f, 0.0f, 0.0f) * scale) * 10.0f;

	vec3f overrall_pos(m_pos);
	for (int i = 0; i < m_txt.size(); i++) {
		if (m_txt[i] == ' ') {						//In case of space
			overrall_pos += m_space;
			continue;
		}

		mat4f mat = RD_Mesh::m_mdl;

		vec2f met = m_txtRndr->GetGlyphRelativeMetrics(m_txt[i]);
		m_letter_spacing = vec3f(
			(scale * 10.0f) * vec3f(2.0f * (met.getX() / met.getY()))
		);

		mat = TranslateMatrix(mat, overrall_pos);

		vec3f nScale = scale * vec3f(met.getX() / met.getY(), 1.0f, 1.0f);
		mat = ScaleMatrix(mat, nScale);

		overrall_pos = overrall_pos + m_letter_spacing;

		m_letters_prop.push_back(mat);
	}

	game->RegisterMesh(reinterpret_cast<RD_Mesh*>(this));
}

void EXP_TextSurface::UpdateMatrices() {
	m_space = (vec3f(1.0f, 0.0f, 0.0f) * RD_Mesh::m_scale) * 10.0f;

	vec3f overrall_pos(RD_Mesh::m_position);
	int i = 0;
	for (auto c : m_txt) {
		//In case of space
		if (c == ' ') {
			overrall_pos += m_space;
			continue;
		}

		mat4f mat = RD_Mesh::m_mdl;

		vec2f met = m_txtRndr->GetGlyphRelativeMetrics(c);
		m_letter_spacing = vec3f(
			(RD_Mesh::m_scale * 10.0f) * vec3f(2.0f * (met.getX() / met.getY()))
		);

		mat = TranslateMatrix(mat, overrall_pos);

		vec3f nScale = RD_Mesh::m_scale * vec3f(met.getX() / met.getY(), 1.0f, 1.0f);
		mat = ScaleMatrix(mat, nScale);

		overrall_pos = overrall_pos + m_letter_spacing;

		m_letters_prop[i] = mat;
		i++;
	}
}

EXP_TextSurface::~EXP_TextSurface() {
	delete m_txtRndr;
}

void EXP_TextSurface::render(RD_Camera* cam) {
	m_shader->useShader();

	m_buffer->BindBuffer();
	m_shader->SetVec3("txtColor", m_color);

	int i = 0;
	for (auto c : m_txt) {
		if (c == ' ') {
			continue;
		}

		m_shader->SetMatrix("model", m_letters_prop[i]);

		m_txtRndr->GetGlyphTexture(c)->BindTexture(0);
		m_rndr->GetRenderingAPI()->Draw(m_buffer);

		i++;
	}

	m_buffer->UnbindBuffer();
}

void EXP_TextSurface::SetPosition(vec3f nPos) {
	EXP_Component::m_pos = nPos;
	RD_Mesh::SetPosition(nPos);

	UpdateMatrices();
}

void EXP_TextSurface::SetRotation(vec3f nRot) {
	EXP_Component::m_rot = nRot;
	RD_Mesh::SetRotation(nRot);

	UpdateMatrices();
}

void EXP_TextSurface::SetScale(vec3f nScale) {
	EXP_Component::m_scale = nScale;
	RD_Mesh::SetScale(nScale);

	UpdateMatrices();
}

void EXP_TextSurface::UseParentMatrix(mat4f parent) {
	RD_Mesh::SetParentMatrix(parent);
}