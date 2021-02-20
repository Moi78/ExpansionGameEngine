#include "pch.h"
#include "EXP_GUI_Text.h"

EXP_GUI_Text::EXP_GUI_Text(EXP_Game* game, const std::string& fontRef, const std::string& text, const int fontSize, vec2f pos, vec2f scale) :
	RD_GUI_Element(), m_mdl(1.0f), m_proj(1.0f) {
	m_game = game;
	
	m_txtrndr = new RD_TextRenderer(game->GetRenderer());
	m_txtrndr->LoadFont(game->GetFilePathByRef(fontRef), fontSize);

	m_text = text;

	m_shader = m_game->GetRenderer()->GetRenderingAPI()->CreateShader();

	m_pos = vec3f(pos.getX(), pos.getY(), 0.0f);
	m_size = vec3f(scale.getX(), scale.getY(), 0.0f);

	mat4f trans = mat4f(1.0f);
	//Ugliest way to proceed, meh, at least it works well
	trans = TranslateMatrix(trans, vec3f(pos.getX() + (pos.getX() / 2), pos.getY() + (pos.getY() / 2), 0.0f));

	mat4f mat_scale = mat4f(1.0f);
	mat_scale = ScaleMatrix(mat_scale, vec3f(scale.getX() / 2, scale.getY() / 2, 0.0f));

	m_mdl = trans * mat_scale;

	float w = (float)m_game->GetRenderer()->GetViewportSize().getX();
	float h = (float)m_game->GetRenderer()->GetViewportSize().getY();

	m_proj = ProjOrtho<float>(1280.0f, 0.0f, 0.0f, (float)1280.0f / (w / h), -1.0f, 1.0f);

	m_surface = std::make_unique<RD_Quad>(game->GetRenderer());
	m_surface->Bufferize();

	RD_GUI_Manager* m_manager = game->GetRenderer()->GetGUI_Manager();
	if (!m_manager->GetGUIshaderManager()->DoMaterialExists("/Shaders/glsl/gui/GUI_Text.frag")) {

		std::cout << "Compiling GUI Text Shader..." << std::endl;
		std::string workingDir = game->GetRenderer()->GetEngineDir();
		m_shader->compileShaderFromFile(
			workingDir + "/Shaders/glsl/gui/GUI_Text.vert",
			workingDir + "/Shaders/glsl/gui/GUI_Text.frag"
		);

		RD_ShaderMaterial* m_mat = new RD_ShaderMaterial(m_shader);
		m_manager->GetGUIshaderManager()->AddMaterialToLib(m_mat, "/Shaders/glsl/gui/GUI_Text.frag");
	}
	else {
		m_shader = m_manager->GetGUIshaderManager()->GetMaterialByName("/Shaders/glsl/gui/GUI_Text.frag")->GetShader();
	}

	game->GetRenderer()->GetGUI_Manager()->RegisterElement(this);
}

EXP_GUI_Text::~EXP_GUI_Text() {
	delete m_txtrndr;
}

void EXP_GUI_Text::RenderElement() {
	m_shader->useShader();

	m_txtrndr->GetGlyphTexture(m_text[0])->BindTexture(0);
	m_shader->SetInt("GlyphTex", 0);

	m_shader->SetMatrix("mdl", m_mdl);
	m_shader->SetMatrix("proj", m_proj);

	m_surface->RenderQuad();
}

void EXP_GUI_Text::RebuildElement() {
	float w = (float)m_game->GetRenderer()->GetViewportSize().getX();
	float h = (float)m_game->GetRenderer()->GetViewportSize().getY();

	m_proj = ProjOrtho<float>((float)1280, 0.0f, 0.0f, (float)1280 / (w / h), -1.0f, 1.0f);
}

void EXP_GUI_Text::SetPosition(vec2f nPos) {
	
}

vec2f EXP_GUI_Text::GetPosition() {
	return vec2f(m_pos.getX(), m_pos.getY());
}