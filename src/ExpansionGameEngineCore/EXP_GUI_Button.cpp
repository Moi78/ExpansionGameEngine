#include "pch.h"
#include "EXP_GUI_Button.h"

EXP_GUI_Button::EXP_GUI_Button(EXP_Game* game, RD_Callback* func, EXP_ButtonImages images, float opacity, float sizex, float sizey, float posx, float posy, bool oneTimeBtn) {
	m_game = game;
	m_opacity = opacity;
	m_pos = vec3f(posx, posy, 0.0f);
	m_size = vec3f(sizex, sizey, 0.0f);
	m_func = func;

	m_oneTimeButton = oneTimeBtn;
	m_released = true;

	//Allocating textures
	std::string workingDir = m_game->GetGameInfo().RootGameContentFolder;

	m_normal = std::make_unique<RD_Texture>();
	m_hover = std::make_unique<RD_Texture>();
	m_pressed = std::make_unique<RD_Texture>();

	m_normal->LoadTexture(workingDir + images.texRefNormal, false);
	m_hover->LoadTexture(workingDir + images.texRefHover, false);
	m_pressed->LoadTexture(workingDir + images.texRefPressed, false);

	//Making matrices
	m_mdl = glm::mat4(1.0f);
	m_mdl = glm::translate(m_mdl, glm::vec3(posx + (sizex / 2), posy + (sizey / 2), 0.0f));
	m_mdl = glm::scale(m_mdl, glm::vec3(sizex / 2, sizey / 2, 0.0f));

	float w = (float)m_game->GetRenderer()->getWindowWidth();
	float h = (float)m_game->GetRenderer()->getWindowHeigh();

	m_proj = glm::ortho(0.0f, (float)1280, (float)1280 / (w / h), 0.0f, -1.0f, 1.0f);

	//Creating surface to draw on
	m_surface = std::make_unique<RD_Quad>();
	m_surface->Bufferize();

	//Getting shader
	RD_GUI_Manager* m_manager = game->GetRenderer()->GetGUI_Manager();
	if (!m_manager->GetGUIshaderManager()->DoMaterialExists("/Shaders/glsl/gui/GUI_ImageTexture.frag")) {
		std::cout << "Compiling GUI Color Cache Shader..." << std::endl;
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

EXP_GUI_Button::~EXP_GUI_Button() {

}

void EXP_GUI_Button::RenderElement() {
	m_gui_shader->useShader();
	m_gui_shader->SetMatrix("mdl", m_mdl);
	m_gui_shader->SetMatrix("proj", m_proj);

	m_gui_shader->SetFloat("alpha", m_opacity);

	float xMouse = m_game->GetInputHandler()->GetGUI_SpaceMousePosX();
	float yMouse = m_game->GetInputHandler()->GetGUI_SpaceMousePosY();
	//Mouse in rect
	if (xMouse >= m_pos.getX() && xMouse <= m_pos.getX() + m_size.getX() && yMouse >= m_pos.getY() && yMouse <= m_pos.getY() + m_size.getY()) {
		//is left button pressed ?
		if (m_game->GetInputHandler()->GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
			//Yes ? Display pressed texture
			m_pressed->BindTexture(GL_TEXTURE0);

			//If it is a one time btn check if it has been released
			if (m_oneTimeButton) {
				if (m_released) {
					//It was released ! Executing callback
					m_released = false;
					m_func->Call();
				}
			}
			else {
				//It is not a one time btn, exec callback on click, at every ticks
				m_func->Call();
			}
		}
		else {
			//Button is technically released
			m_released = true;
			//But hovered
			m_hover->BindTexture(GL_TEXTURE0);
		}
	}
	else {
		m_normal->BindTexture(GL_TEXTURE0);
	}

	m_gui_shader->SetInt("imageTexture", 0);
	m_surface->RenderQuad();
}

void EXP_GUI_Button::RebuildElement() {
	float w = (float)m_game->GetRenderer()->getWindowWidth();
	float h = (float)m_game->GetRenderer()->getWindowHeigh();

	m_proj = glm::ortho(0.0f, (float)1280, (float)1280 / (w / h), 0.0f, -1.0f, 1.0f);
}

void EXP_GUI_Button::SetPosition(vec3f nPos) {
	m_pos = nPos;

	m_mdl = glm::mat4(1.0f);
	m_mdl = glm::translate(m_mdl, glm::vec3(m_pos.getX() + (m_size.getX() / 2), m_pos.getY() + (m_size.getY() / 2), 0.0f));
	m_mdl = glm::scale(m_mdl, glm::vec3(m_size.getX() / 2, m_size.getY() / 2, 0.0f));
}

vec3f EXP_GUI_Button::GetPosition() {
	return m_pos;
}