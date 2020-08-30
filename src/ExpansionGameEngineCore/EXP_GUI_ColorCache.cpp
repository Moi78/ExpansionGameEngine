#include "pch.h"
#include "EXP_GUI_ColorCache.h"

EXP_GUI_ColorCache::EXP_GUI_ColorCache(EXP_Game* game, vec3f color, float opacity, float sizex, float sizey, float posx, float posy) : RD_GUI_Element() {
	m_color = color;
	m_opacity = opacity;
	m_game = game;

	m_size = vec3f(sizex, sizey, 0);
	m_pos = vec3f(posx, posy, 0);

	RD_GUI_Manager* m_manager = game->GetRenderer()->GetGUI_Manager();

	int w = game->GetRenderer()->getWindowWidth();
	int h = game->GetRenderer()->getWindowHeigh();

	m_mdl = glm::mat4(1.0f);
	//Ugliest way to proceed, meh, at least it works well
	m_mdl = glm::translate(m_mdl, glm::vec3(posx + (sizex / 2), posy + (sizey / 2), 0.0f));
	m_mdl = glm::scale(m_mdl, glm::vec3(sizex / 2, sizey / 2, 0.0f));

	m_proj = glm::ortho(0.0f, 1280.0f, (float)1280.0f / (w / h), 0.0f, -1.0f, 1.0f);

	m_surface = std::make_unique<RD_Quad>();
	m_surface->Bufferize();

	if (!m_manager->GetGUIshaderManager()->DoMaterialExists("/Shaders/glsl/gui/GUI_ColorCache.frag")) {
		std::cout << "Compiling GUI Color Cache Shader..." << std::endl;
		std::string workingDir = game->GetRenderer()->GetEngineDir();
		m_gui_shader->compileShaderFromFile(workingDir + "/Shaders/glsl/gui/GUI_ColorCache.vert", workingDir + "/Shaders/glsl/gui/GUI_ColorCache.frag");

		RD_ShaderMaterial* m_mat = new RD_ShaderMaterial(m_gui_shader);
		m_manager->GetGUIshaderManager()->AddMaterialToLib(m_mat, "/Shaders/glsl/gui/GUI_ColorCache.frag");
	}
	else {
		m_gui_shader = m_manager->GetGUIshaderManager()->GetMaterialByName("/Shaders/glsl/gui/GUI_ColorCache.frag")->GetShader();
	}

	game->GetRenderer()->GetGUI_Manager()->RegisterElement(this);
}

EXP_GUI_ColorCache::~EXP_GUI_ColorCache() {

}

void EXP_GUI_ColorCache::RenderElement() {
	m_gui_shader->useShader();
	m_gui_shader->SetVec3("color", m_color);
	m_gui_shader->SetFloat("alpha", m_opacity);

	m_gui_shader->SetMatrix("mdl", m_mdl);
	m_gui_shader->SetMatrix("proj", m_proj);

	m_surface->RenderQuad();
}

void EXP_GUI_ColorCache::RebuildElement() {
	float w = (float)m_game->GetRenderer()->getWindowWidth();
	float h = (float)m_game->GetRenderer()->getWindowHeigh();

	m_proj = glm::ortho(0.0f, (float)1280, (float)1280 / (w / h), 0.0f, -1.0f, 1.0f);
}

void EXP_GUI_ColorCache::SetPosition(vec3f nPos) {
	m_pos = nPos;

	glm::mat4 trans = glm::mat4(1.0f);
	//Ugliest way to proceed, meh, at least it works well
	trans = glm::translate(trans, glm::vec3(m_pos.getX() + (m_size.getX() / 2), m_pos.getY() + (m_size.getY() / 2), 0.0f));

	glm::mat4 scale = glm::mat4(1.0f);
	scale = glm::scale(scale, glm::vec3(m_size.getX() / 2, m_size.getY() / 2, 0.0f));

	m_mdl = trans * scale;
}

vec3f EXP_GUI_ColorCache::GetPosition() {
	return m_pos;
}