#include "pch.h"
#include "EXP_GUI_ImageTexture.h"

EXP_GUI_ImageTexture::EXP_GUI_ImageTexture(EXP_Game* game, std::string texRef, float opacity, float sizex, float sizey, float posx, float posy) : RD_GUI_Element(), m_mdl(1.0f), m_proj(1.0f) {
	m_opacity = opacity;
	m_game = game;

	m_gui_shader = m_game->GetRenderer()->GetRenderingAPI()->CreateShader();

	m_pos = vec3f(posx, posy, 0);
	m_size = vec3f(sizex, sizey, 0);

	mat4f trans = mat4f(1.0f);
	//Ugliest way to proceed, meh, at least it works well
	trans = TranslateMatrix(trans, vec3f(posx + (sizex / 2), posy + (sizey / 2), 0.0f));

	mat4f scale = mat4f(1.0f);
	scale = ScaleMatrix(scale, vec3f(sizex / 2, sizey / 2, 0.0f));

	m_mdl = trans * scale;

	float w = (float)m_game->GetRenderer()->getWindowWidth();
	float h = (float)m_game->GetRenderer()->getWindowHeigh();

	m_proj = ProjOrtho<float>((float)1280, 0.0f, 0.0f, (float)1280 / (w / h), -1.0f, 1.0f);

	m_image = game->GetRenderer()->GetRenderingAPI()->CreateTexture();

	std::string ref = game->GetFilePathByRef(texRef);

	m_image->LoadTexture(ref, false);

	m_surface = std::make_unique<RD_Quad>(game->GetRenderer());
	m_surface->Bufferize();

	RD_GUI_Manager* m_manager = game->GetRenderer()->GetGUI_Manager();

	if (!m_manager->GetGUIshaderManager()->DoMaterialExists("/Shaders/glsl/gui/GUI_ImageTexture.frag")) {
		std::cout << "Compiling GUI ImageTexture Shader..." << std::endl;
		std::string workingDir = game->GetRenderer()->GetEngineDir();
		m_gui_shader->compileShaderFromFile(workingDir + "/Shaders/glsl/gui/GUI_ImageTexture.vert", workingDir + "/Shaders/glsl/gui/GUI_ImageTexture.frag");

		RD_ShaderMaterial* m_mat = new RD_ShaderMaterial(m_gui_shader);
		m_manager->GetGUIshaderManager()->AddMaterialToLib(m_mat, "/Shaders/glsl/gui/GUI_ImageTexture.frag");
	}
	else {
		m_gui_shader = m_manager->GetGUIshaderManager()->GetMaterialByName("/Shaders/glsl/gui/GUI_ImageTexture.frag")->GetShader();
	}

	game->GetRenderer()->GetGUI_Manager()->RegisterElement(this);
}

EXP_GUI_ImageTexture::~EXP_GUI_ImageTexture() {

}

void EXP_GUI_ImageTexture::RenderElement() {
	m_gui_shader->useShader();

	m_image->BindTexture(0);
	m_gui_shader->SetInt("imageTexture", 0);

	m_gui_shader->SetFloat("alpha", m_opacity);

	m_gui_shader->SetMatrix("mdl", m_mdl);
	m_gui_shader->SetMatrix("proj", m_proj);

	m_surface->RenderQuad();
}

void EXP_GUI_ImageTexture::RebuildElement() {
	float w = (float)m_game->GetRenderer()->getWindowWidth();
	float h = (float)m_game->GetRenderer()->getWindowHeigh();

	m_proj = ProjOrtho<float>((float)1280, 0.0f, 0.0f, (float)1280 / (w / h), -1.0f, 1.0f);
}

void EXP_GUI_ImageTexture::SetPosition(vec3f nPos) {
	m_pos = nPos;

	mat4f trans = mat4f(1.0f);
	//Ugliest way to proceed, meh, at least it works well
	trans = TranslateMatrix(trans, vec3f(m_pos.getX() + (m_size.getX() / 2), m_pos.getY() + (m_size.getY() / 2), 0.0f));

	mat4f scale = mat4f(1.0f);
	scale = ScaleMatrix(scale, vec3f(m_size.getX() / 2, m_size.getY() / 2, 0.0f));

	m_mdl = trans * scale;
}

vec3f EXP_GUI_ImageTexture::GetPosition() {
	return m_pos;
}