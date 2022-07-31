#include "RaindropRenderer.h"

RaindropRenderer::RaindropRenderer(std::shared_ptr<RD_API> api, const int w, const int h, std::string wname) {
	m_api = api;

	m_width = w;
	m_height = h;
	m_wname = wname;
}

RaindropRenderer::~RaindropRenderer() {

}

bool RaindropRenderer::InitRenderer() {
	if (!m_api->InitAPI(m_wname, m_width, m_height)) {
		return false;
	}

	//BuildPresentPipeline();
    m_api->GetWindowingSystem()->BuildBlitPipeline();

	if(!m_api->MakeFramebuffers()) {
		return false;
	}

	return true;
}

void RaindropRenderer::RenderScene() {
	m_api->GetWindowingSystem()->Present();
}

void RaindropRenderer::UpdateWindow() {
	m_api->GetWindowingSystem()->PollEvents();
}

bool RaindropRenderer::WantToClose() {
	if(!m_api->GetWindowingSystem()->WantToClose()) {
		return false;
	} else {
		m_api->ProperQuit();
		return true;
	}
}
