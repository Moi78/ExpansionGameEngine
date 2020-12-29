#include "pch.h"
#include "RD_GUI_Manager.h"

RD_GUI_Manager::RD_GUI_Manager(RaindropRenderer* rndr) {
	m_rndr = rndr;

	m_framebuffer = rndr->GetRenderingAPI()->CreateFrameBuffer(rndr->getWindowWidth(), rndr->getWindowHeigh());
	m_framebuffer->ConfigureRenderbuffer(DEPTH_STENCIL_COMPONENT, DEPTH24_STENCIL8_ATTACHEMENT);

	m_matLib = std::make_unique<RD_MaterialLibrary>();

	m_gui_elements.push_back(new ScreenCleaner(rndr, rndr->GetEngineDir()));
}

RD_GUI_Manager::~RD_GUI_Manager() {
	DeleteFrambuffer();
	std::cout << "GUI Framebuffer deleted." << std::endl;

	for (auto* elem : m_gui_elements) {
		delete elem;
	}
	m_gui_elements.clear();
	std::cout << "GUI Elements array were cleared and deleted." << std::endl;
}

void RD_GUI_Manager::InitManager() {
	CreateFramebuffer();

	std::cout << "GUI Initialized" << std::endl;
}

void RD_GUI_Manager::CreateFramebuffer() {
	m_framebuffer->AddAttachement(IMGFORMAT_RGBA);
	m_framebuffer->BuildFBO();
}

void RD_GUI_Manager::DeleteFrambuffer() {
	delete m_framebuffer;
}

void RD_GUI_Manager::RebuildFramebuffer() {
	int w = m_rndr->getWindowWidth();
	int h = m_rndr->getWindowHeigh();

	m_framebuffer->ChangeFramebufferSize(w, h);

	for (auto elem : m_gui_elements) {
		elem->RebuildElement();
	}
}

void RD_GUI_Manager::RenderScreen() {
	m_framebuffer->BindFBO();
	m_rndr->GetRenderingAPI()->Clear(DEPTH_BUFFER | COLOR_BUFFER);
	
	for (auto elem : m_gui_elements) {
		elem->RenderElement();
	}

	m_framebuffer->UnbindFBO();
}

RD_Texture* RD_GUI_Manager::GetScreenTexture() {
	return m_framebuffer->GetAttachementByIndex(0);
}

void RD_GUI_Manager::RegisterElement(RD_GUI_Element* elem) {
	std::cout << "Registering GUI element." << std::endl;
	m_gui_elements.push_back(elem);
}

void RD_GUI_Manager::UnregisterElement(RD_GUI_Element* elem) {
	int index = GetElemIndex<RD_GUI_Element*>(m_gui_elements, elem);
	if (index != -1) {
		delete m_gui_elements[index];
		m_gui_elements.erase(m_gui_elements.begin() + index);
	}
}

RD_MaterialLibrary* RD_GUI_Manager::GetGUIshaderManager() {
	return m_matLib.get();
}

//--------------------------ScreenCleaner---------------------------------

ScreenCleaner::ScreenCleaner(RaindropRenderer* rndr, std::string workingDir) : RD_GUI_Element() {
	m_gui_shader = rndr->GetRenderingAPI()->CreateShader();
	
	std::cout << "Compiling GUI_Clean shader..." << std::endl;
	m_gui_shader->compileShaderFromFile(workingDir + "/Shaders/glsl/gui/GUI_Clean.vert", workingDir + "/Shaders/glsl/gui/GUI_Clean.frag");

	m_surface = std::make_unique<RD_Quad>(rndr);
	m_surface->Bufferize();
}

ScreenCleaner::~ScreenCleaner() {
	delete m_gui_shader;
}

void ScreenCleaner::RenderElement() {
	m_gui_shader->useShader();

	m_surface->RenderQuad();
}