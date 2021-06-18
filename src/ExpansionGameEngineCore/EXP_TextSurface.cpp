#include "pch.h"
#include "EXP_TextSurface.h"

EXP_TextSurface::EXP_TextSurface(
	EXP_Game* game,
	const std::string& text,
	const int size,
	const std::string& font,
	vec3f pos, vec3f rot, vec3f scale,
	vec3f color) :

	EXP_Component(pos, rot, scale),
	RD_Mesh(game->GetRenderer(), nullptr, pos, rot, scale, true)
{
	m_game = game;
	RD_Mesh::m_mat = game->GetRenderer()->GetMaterialLibrary()->GetMaterialByName("text");
	m_color = color;
	m_mat->RegisterMeshReference(reinterpret_cast<RD_Mesh*>(this));

	auto* txt_mgr = game->GetRenderer()->GetTxtRendererManager();

	if (txt_mgr->DoRessourceExists(font)) {
		m_txtRndr = txt_mgr->GetRessourceByName(font);
	}
	else {
		m_txtRndr = new RD_TextRenderer(game->GetRenderer());
		m_txtRndr->LoadFont(game->GetFilePathByRef(font), size);
		txt_mgr->AddRessource(m_txtRndr, font);
	}

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
			(scale * 10.0f) * vec3f(2.0f * std::clamp(met.getX() / met.getY(), 0.0f, 1.0f))
		);

		mat = TranslateMatrix(mat, overrall_pos);

		vec3f nScale = scale * vec3f(std::clamp(met.getX() / met.getY(), 0.0f, 1.0f), std::clamp(met.getY() / met.getX(), 0.0f, 1.0f), 1.0f);
		mat = ScaleMatrix(mat, nScale);

		overrall_pos = overrall_pos + m_letter_spacing;

		m_letters_prop.push_back(mat);
	}

	game->RegisterMesh(reinterpret_cast<RD_Mesh*>(this));
}

void EXP_TextSurface::UpdateMatrices() {
	m_space = (vec3f(1.0f, 0.0f, 0.0f) * RD_Mesh::m_scale) * 10.0f;

	vec3f overrall_pos = vec3f();
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
			(RD_Mesh::m_scale * 10.0f) * vec3f(2.0f * std::clamp(met.getX() / met.getY(), 0.0f, 1.0f))
		);

		vec3f nScale = RD_Mesh::m_scale * vec3f(
			std::clamp(met.getX() / met.getY(), 0.0f, 1.0f),
			std::clamp(met.getY() / met.getX(), 0.0f, 1.0f), 1.0f
		);

		mat = TranslateMatrix(mat, overrall_pos);

		mat = ScaleMatrix(mat, nScale);

		overrall_pos = overrall_pos + m_letter_spacing;

		m_letters_prop[i] = mat;
		i++;
	}
}

EXP_TextSurface::~EXP_TextSurface() {}

void EXP_TextSurface::render() {
	m_buffer->BindBuffer();
	RD_Mesh::m_mat->GetShader()->SetVec3("txtColor", m_color);

	int i = 0;
	for (auto c : m_txt) {
		if (c == ' ') {
			continue;
		}

		m_rndr->PushModelMatrix(m_letters_prop[i]);

		m_txtRndr->GetGlyphTexture(c)->BindTexture(0);
		m_mat->GetShader()->SetInt("glyph", 0);
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