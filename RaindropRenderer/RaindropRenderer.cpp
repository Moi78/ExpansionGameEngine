#include "RaindropRenderer.h"

RaindropRenderer::RaindropRenderer(std::shared_ptr<RD_API> api, std::shared_ptr<RD_RenderingPipeline> rpline, const int w, const int h, std::string wname) {
	m_api = api;
    m_rpline = rpline;

	m_width = w;
	m_height = h;
	m_wname = wname;
}

RaindropRenderer::~RaindropRenderer() {
    m_meshes.clear();
}

bool RaindropRenderer::InitRenderer() {
	if (!m_api->InitAPI(m_wname, m_width, m_height)) {
		return false;
	}

    m_api->GetWindowingSystem()->BuildBlitPipeline();

	if(!m_api->MakeFramebuffers()) {
		return false;
	}

    m_rpline->InitRenderingPipeline();

    m_resize_cbck = std::make_shared<RD_Callback>(CL_VDFUNCPTR(RaindropRenderer::Resize));
    m_api->GetWindowingSystem()->SetExternalResizeCallback(m_resize_cbck);

    m_mesh = std::make_shared<RD_Mesh>(m_api, vec3(), vec3(), vec3(2.0f, 2.0f, 2.0f));
    m_mesh->LoadMesh("./mdl/monkey.msh");
    RegisterMesh(m_mesh);

	return true;
}

void RaindropRenderer::RenderScene() {
    m_mesh->Rotate(vec3(0.0f, 0.0f, 0.5f));

    m_rpline->RenderScene(m_meshes);

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

void RaindropRenderer::Resize() {
    int w = m_api->GetWindowingSystem()->GetWidth();
    int h = m_api->GetWindowingSystem()->GetHeight();

    m_rpline->Resize(w, h);
}

void RaindropRenderer::RegisterMesh(std::shared_ptr<RD_Mesh> mesh) {
    std::cout << "Registered new mesh" << std::endl;

    m_meshes.push_back(mesh);
}