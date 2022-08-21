#include "RaindropRenderer.h"

RaindropRenderer::RaindropRenderer(std::shared_ptr<RD_API> api, std::shared_ptr<RD_RenderingPipeline> rpline, const int w, const int h, std::string wname) {
	m_api = api;
    m_rpline = rpline;

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

    m_api->GetWindowingSystem()->BuildBlitPipeline();

	if(!m_api->MakeFramebuffers()) {
		return false;
	}

    m_rpline->InitRenderingPipeline();

    m_verticies = m_api->CreateIndexedVertexBuffer();

    auto vData = MakeVertexData(
            { vec3(-1.0, 1.0, 0.0), vec3(-1.0, -1.0, 0.0), vec3(1.0, 1.0, 0.0), vec3(1.0, -1.0, 0.0) },
            { vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0) },
            { vec2(0.0, 1.0), vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 0.0)}
    );

    std::vector<uint32_t> indices = {
            0, 1, 2,
            1, 3, 2
    };

    m_verticies->FillBufferData(vData, indices);

    m_verts2 = m_api->CreateIndexedVertexBuffer();

    auto vData2 = MakeVertexData(
            { vec3(-1.0, 1.0, 0.5), vec3(-1.0, -1.0, 0.5), vec3(1.0, 1.0, 0.5), vec3(1.0, -1.0, 0.5) },
            { vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0) },
            { vec2(0.0, 1.0), vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 0.0)}
    );

    m_verts2->FillBufferData(vData2, indices);

	return true;
}

void RaindropRenderer::RenderScene() {
    std::vector<std::shared_ptr<RD_IndexedVertexBuffer>> vbuff = {m_verts2, m_verticies};
    m_rpline->RenderScene(vbuff);

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
