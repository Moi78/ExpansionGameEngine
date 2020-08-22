#include "pch.h"
#include "EXP_GUI_ColorCache.h"

EXP_GUI_ColorCache::EXP_GUI_ColorCache(EXP_Game* game, vec3f color, float opacity, float sizex, float sizey, float posx, float posy) : RD_GUI_Element() {
	m_color = color;
	m_opacity = opacity;
	m_game = game;

	RD_GUI_Manager* m_manager = game->GetRenderer()->GetGUI_Manager();

	m_mdl = glm::mat4(1.0f);
	//Ugliest way to proceed, meh, at least it works well
	m_mdl = glm::translate(m_mdl, glm::vec3(posx + (sizex / 2), posy + (sizey / 2), 0.0f));
	m_mdl = glm::scale(m_mdl, glm::vec3(sizex / 2, sizey / 2, 0.0f));

	m_proj = glm::ortho(0.0f, (float)m_game->GetRenderer()->getWindowWidth(), (float)m_game->GetRenderer()->getWindowHeigh(), 0.0f, -1.0f, 1.0f);

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
	m_proj = glm::ortho(0.0f, (float)m_game->GetRenderer()->getWindowWidth(), (float)m_game->GetRenderer()->getWindowHeigh(), 0.0f, -1.0f, 1.0f);
}