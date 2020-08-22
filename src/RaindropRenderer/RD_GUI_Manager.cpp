#include "pch.h"
#include "RD_GUI_Manager.h"

RD_GUI_Manager::RD_GUI_Manager(RaindropRenderer* rndr) {
	m_rndr = rndr;

	m_framebuffer = NULL;
	m_gui_tex = NULL;
	m_renderbuffer = NULL;

	m_matLib = std::make_unique<RD_MaterialLibrary>();

	m_gui_elements.push_back(new ScreenCleaner(rndr->GetEngineDir()));
}

RD_GUI_Manager::~RD_GUI_Manager() {
	DeleteFrambuffer();
	std::cout << "GUI Framebuffer deleted." << std::endl;

	for (auto elem : m_gui_elements) {
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
	int w = m_rndr->getWindowWidth();
	int h = m_rndr->getWindowHeigh();

	//Allocating framebuffer
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	//Allocating "screen" texture
	glGenTextures(1, &m_gui_tex);
	glBindTexture(GL_TEXTURE_2D, m_gui_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gui_tex, 0);

	//Allocating render buffer
	glGenRenderbuffers(1, &m_renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL, GL_RENDERBUFFER, m_renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: GUI Framebuffer not complete." << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RD_GUI_Manager::DeleteFrambuffer() {
	//De-allocating "screen" texture
	glDeleteTextures(1, &m_gui_tex);

	//De-allocating framebuffer
	glDeleteFramebuffers(1, &m_framebuffer);

	//De-allocating renderbuffer
	glDeleteRenderbuffers(GL_RENDERBUFFER, &m_renderbuffer);
}

void RD_GUI_Manager::RebuildFramebuffer() {
	DeleteFrambuffer();
	CreateFramebuffer();

	for (auto elem : m_gui_elements) {
		elem->RebuildElement();
	}
}

void RD_GUI_Manager::RenderScreen() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (auto elem : m_gui_elements) {
		elem->RenderElement();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int RD_GUI_Manager::GetScreenTexture() {
	return m_gui_tex;
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

ScreenCleaner::ScreenCleaner(std::string workingDir) : RD_GUI_Element() {
	std::cout << "Compiling GUI_Clean shader..." << std::endl;
	m_gui_shader->compileShaderFromFile(workingDir + "/Shaders/glsl/gui/GUI_Clean.vert", workingDir + "/Shaders/glsl/gui/GUI_Clean.frag");

	m_surface = std::make_unique<RD_Quad>();
	m_surface->Bufferize();
}

ScreenCleaner::~ScreenCleaner() {

}

void ScreenCleaner::RenderElement() {
	m_gui_shader->useShader();

	m_surface->RenderQuad();
}