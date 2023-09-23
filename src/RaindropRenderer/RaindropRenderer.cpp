#include "RaindropRenderer.h"

RaindropRenderer::RaindropRenderer(std::shared_ptr<RD_API> api, std::shared_ptr<RD_RenderingPipeline> rpline, const int w, const int h, std::string wname) {
	m_api = api;
    m_rpline = rpline;

	m_width = w;
	m_height = h;
	m_wname = wname;

    m_viewCam = std::make_shared<RD_Camera>(m_api, 0.0f, vec3(), vec3(), 0.0f, 0.0f);
}

RaindropRenderer::~RaindropRenderer() {
    m_meshes.clear();
}

bool RaindropRenderer::InitRenderer(std::string enginePath) {
    m_enginePath = enginePath;

	if (!m_api->InitAPI(m_wname, m_width, m_height)) {
		return false;
	}

    m_api->GetWindowingSystem()->BuildBlitPipeline(enginePath);

	if(!m_api->MakeFramebuffers()) {
		return false;
	}

    m_rpline->InitRenderingPipeline(enginePath);

    m_resize_cbck = std::make_shared<RD_Callback>(CL_VDFUNCPTR(RaindropRenderer::Resize));
    m_api->GetWindowingSystem()->SetExternalResizeCallback(m_resize_cbck);

    m_api->GetWindowingSystem()->InitImgui(enginePath);

	return true;
}

void RaindropRenderer::RenderScene() {
    m_rpline->RenderShadows(m_materials, m_dirLights);
    m_rpline->RenderScene(m_materials, m_viewCam);

    m_rpline->ExecRender();
}

void RaindropRenderer::Present() {
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
    m_viewCam->UpdateProj();

    if(m_ext_resize_cbck.has_value()) {
        m_ext_resize_cbck.value().Call();
    }
}

void RaindropRenderer::RegisterMesh(std::shared_ptr<RD_Mesh> mesh) {
    std::cout << "Registered new mesh" << std::endl;

    m_meshes.push_back(mesh);
}

void RaindropRenderer::RegisterDirLight(std::shared_ptr<RD_DirLight> dlight) {
    std::cout << "Registered new dir light" << std::endl;

    m_dirLights.push_back(dlight);
    UpdateLighting(false, true);
}

void RaindropRenderer::UpdateLighting(bool pLighting, bool dLighting) {
    int i = 0;
    int shadowCasterCount = 0;

    if(dLighting) {
        for (auto &dl: m_dirLights) {
            m_rpline->PushDirLight(dl, i);

            if (dl->IsShadowCaster()) {
                m_rpline->PushLightMat(dl->GetTransMat(), shadowCasterCount);
                shadowCasterCount++;
            }

            i++;
        }
        m_rpline->SetNumberOfShadowFB(shadowCasterCount);
    }

    if(pLighting) {
        i = 0;
        for (auto &pl: m_pointLights) {
            m_rpline->PushPointLight(pl, i);

            i++;
        }
    }

    RD_CasterCount ccount{};
    ccount.nDLights = static_cast<int>(m_dirLights.size());
    ccount.nPlights = static_cast<int>(m_pointLights.size());

    m_rpline->PushCasterCount(ccount);
}

void RaindropRenderer::RegisterPointLight(std::shared_ptr<RD_PointLight> plight) {
    std::cout << "Registered new point light" << std::endl;

    m_pointLights.push_back(plight);
    UpdateLighting(true, false);
}

std::string RaindropRenderer::GetEnginePath() {
    return m_enginePath;
}

void RaindropRenderer::RegisterMaterial(std::shared_ptr <RD_Material> mat) {
    m_materials.push_back(mat);
}

std::shared_ptr<RD_API> RaindropRenderer::GetAPI() {
    return m_api;
}

std::shared_ptr<RD_Pipeline> RaindropRenderer::CreatePipeline(std::shared_ptr<RD_ShaderLoader> &shader) {
    auto pline = m_api->CreatePipeline(m_rpline->GetBaseRenderpass(), shader);
    pline->ConfigurePushConstant(16 * sizeof(float) + sizeof(int32_t));

    m_rpline->SetupPipeline(pline);

    return pline;
}

void RaindropRenderer::SetCurrentCamera(std::shared_ptr<RD_Camera> cam) {
    m_viewCam = cam;
}

void RaindropRenderer::SetupSkeleton(std::shared_ptr<RD_Skeleton> skel) {
    m_rpline->SetupSkeleton(skel);
}

void RaindropRenderer::SetExtResizeCallback(RD_Callback cbck) {
    m_ext_resize_cbck = cbck;
}
