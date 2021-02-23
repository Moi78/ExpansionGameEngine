#include "pch.h"
#include "RaindropRenderer.h"

#include "RD_DirLight.h"
#include "RD_Mesh.h"
#include "RD_Quad.h"
#include "RD_Camera.h"
#include "RD_MaterialLibrary.h"
#include "RD_GUI_Manager.h"
#include "RD_PostProcess.h"

#include "RD_RenderingAPI.h"
#include "RD_RenderingAPI_GL.h"

RaindropRenderer::RaindropRenderer(int w, int h, std::string windowName, API api, Pipeline pline, int maxFramerate, bool minInit, std::string engineDir) : m_vp_size(w, h), m_vp_pos(0.0f, 0.0f) {
	FillFeaturesArray();

	m_vsync = true;
	
	m_engineDir = std::move(engineDir);

	if (api == API::OPENGL) {
		m_api = std::make_unique<RD_RenderingAPI_GL>(this);
	}

	m_error_flag = false;
	m_resize_override = false;

	assert(m_api != nullptr && "ERROR: No 3D-API selected.");

	m_api->InitializeAPI(w, h, std::move(windowName));

	m_frmLmt = std::make_unique<RD_FrameLimiter>(maxFramerate);
	m_pipeline = pline;

	//Shader Compiling
	if (m_pipeline == Pipeline::LAMBERT_ENGINE) {
		std::cout << "Compiling main shaders, lambert shading model..." << std::endl;

		m_shadowShader = m_api->CreateShader();
		m_shadowShader->compileShaderFromFile(
			m_engineDir + "/Shaders/glsl/Shadow.vert",
			m_engineDir + "/Shaders/glsl/Shadow.frag"
		);

		m_light_shader = m_api->CreateShader();
		m_light_shader->compileShaderFromFile(
			m_engineDir + "/Shaders/glsl/Light.vert",
			m_engineDir + "/Shaders/glsl/Light.frag"
		);

		m_beauty_shader = m_api->CreateShader();
		m_beauty_shader->compileShaderFromFile(
			m_engineDir + "/Shaders/glsl/Beauty.vert",
			m_engineDir + "/Shaders/glsl/Beauty.frag"
		);

		m_CurrentShader = m_light_shader;

		ambientStrength = 1.0f;
	}
	else {
		std::cout << "Compiling main shaders, PBR shading model..." << std::endl;

		m_shadowShader = m_api->CreateShader();
		m_shadowShader->compileShaderFromFile(
			m_engineDir + "/Shaders/glsl/Shadow.vert",
			m_engineDir + "/Shaders/glsl/Shadow.frag");

		m_light_shader = m_api->CreateShader();
		m_light_shader->compileShaderFromFile(
			m_engineDir + "/Shaders/glsl/pbr/Light.vert",
			m_engineDir + "/Shaders/glsl/pbr/Light.frag");

		m_beauty_shader = m_api->CreateShader();
		m_beauty_shader->compileShaderFromFile(
			m_engineDir + "/Shaders/glsl/Beauty.vert",
			m_engineDir + "/Shaders/glsl/Beauty.frag"
		);

		m_ssr_shader = m_api->CreateShader();
		m_ssr_shader->compileShaderFromFile(
			m_engineDir + "/Shaders/glsl/pbr/SSR.vert",
			m_engineDir + "/Shaders/glsl/pbr/SSR.frag"
		);

		m_ssao_shader = m_api->CreateShader();
		m_ssao_shader->compileShaderFromFile(
			m_engineDir + "/Shaders/glsl/pbr/SSAO.vert",
			m_engineDir + "/Shaders/glsl/pbr/SSAO.frag"
		);

		m_ssao_blur_shader = m_api->CreateShader();
		m_ssao_blur_shader->compileShaderFromFile(
			m_engineDir + "/Shaders/glsl/pbr/SSAO_Blur.vert", 
			m_engineDir + "/Shaders/glsl/pbr/SSAO_Blur.frag"
		);

		m_bloom = m_api->CreateShader();
		m_bloom->compileShaderFromFile(
			m_engineDir + "/Shaders/glsl/pbr/bloom.vert",
			m_engineDir + "/Shaders/glsl/pbr/bloom.frag"
		);
		
		m_CurrentShader = m_light_shader;

		ambientStrength = 0.001f;

		m_ssao_u = m_api->CreateUniformBuffer(64 * sizeof(float), 6);

		GenerateSSAOKernels();
		GenerateSSAONoise();
	}

	m_shadowCalc = m_api->CreateShader();
	m_shadowCalc->compileShaderFromFile(
		m_engineDir + "/Shaders/glsl/ShadowCalc.vert",
		m_engineDir + "/Shaders/glsl/ShadowCalc.frag"
	);

	m_pointLight_u = m_api->CreateUniformBuffer(243 * (8 * 4 + sizeof(int)), 3);
	m_dirLights_u = m_api->CreateUniformBuffer(10 * ((7 * 4) + sizeof(int)), 4);
	m_ambient_u = m_api->CreateUniformBuffer(17, 2);

	m_shadows_buffer = m_api->CreateFrameBuffer(GetViewportSize().getX(), GetViewportSize().getY(), true);
	m_shadows_buffer->AddAttachement(IMGFORMAT_RGB16F);
	m_shadows_buffer->BuildFBO();
	
	m_defTex = m_api->CreateTexture();
	m_defTex->LoadTexture(m_engineDir + "/Textures/defTex.png");

	m_blankTexture = m_api->CreateTexture();
	m_blankTexture->GenerateColorTex(vec3f(1.0f, 1.0f, 1.0f));

	if constexpr (RENDER_DEBUG_ENABLED) {
		RD_ShaderLoader* shad = m_api->CreateShader();
		shad->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Debug.vert", m_engineDir + "/Shaders/glsl/Debug.frag");
		m_dbgMat = new RD_ShaderMaterial(shad);

		m_DBG_light_mdl = std::make_unique<RD_Mesh>(this, m_dbgMat, vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.3f, 0.3f, 0.3f));
		m_DBG_light_mdl->loadMesh(m_engineDir + "/Meshes/Light.msh");
	}

	ambientColor = vec3f(1.0f, 1.0f, 1.0f);

	if (m_pipeline == Pipeline::LAMBERT_ENGINE) {
		CreateGbuff();
	}
	else {
		CreateGbuff_PBR();
	}

	m_gui_manager = std::make_unique<RD_GUI_Manager>(this);
	m_gui_manager->InitManager();

	m_quad = std::make_unique<RD_Quad>(this);
	m_quad->Bufferize();

	m_matlib = std::make_unique<RD_MaterialLibrary>();

	UpdateAmbientLighting();
}

RaindropRenderer::~RaindropRenderer() {
	m_matlib->ClearLibrary();

	m_pt_lights.clear();
	m_DirLights.clear();
	m_meshes.clear();

	delete m_gbuffer;
	delete m_light_pprocess;

	//Deleting common shaders
	delete m_shadowShader;
	delete m_light_shader;
	delete m_beauty_shader;
	delete m_dbgMat;
	delete m_shadowCalc;
	
	//Deleting textures
	delete m_defTex;
	delete m_blankTexture;

	//Deleting uniform buffers
	delete m_pointLight_u;
	delete m_dirLights_u;
	delete m_ambient_u;

	//PBR related deletion
	if (m_pipeline == Pipeline::PBR_ENGINE) {
		delete m_ssao_buffer;

		delete m_ssr_shader;
		delete m_ssao_shader;
		delete m_ssao_blur_shader;
		delete m_bloom;

		delete m_ssao_noise_tex;
		delete m_bloom_buffera;
		delete m_bloom_bufferb;

		delete m_ssao_u;
	}
}

RD_RenderingAPI* RaindropRenderer::GetRenderingAPI() const {
	return m_api.get();
}

void RaindropRenderer::ClearWindow(vec3f refreshColor) const {
	m_api->GetWindowingSystem()->PollEvents();

	m_frmLmt->start();
	m_api->Clear(COLOR_BUFFER | DEPTH_BUFFER);
}

void RaindropRenderer::SwapWindow() {
	m_api->GetWindowingSystem()->SwapWindow();
	m_error_flag = false;

	m_frmLmt->stop();
	//m_frmLmt->WaitAll();
}

bool RaindropRenderer::WantToClose() const {
	if (m_api->GetWindowingSystem()->WantToClose()) {
		return true;
	}
	else {
		return false;
	}
}

int RaindropRenderer::getWindowHeigh() const {
	return m_api->GetWindowingSystem()->GetHeight();
}

int RaindropRenderer::getWindowWidth() const {
	return m_api->GetWindowingSystem()->GetWidth();
}

void RaindropRenderer::SetAmbientStrength(float strength) {
	ambientStrength = strength;

	UpdateAmbientLighting();
}

void RaindropRenderer::SetAmbientColor(const vec3f& nColor) {
	ambientColor = nColor;

	UpdateAmbientLighting();
}

void RaindropRenderer::UpdateAmbientLighting() {
	if (!IsFeatureEnabled(RendererFeature::Ambient))
		return;

	/*m_CurrentShader->SetFloat("AmbientStrength", ambientStrength);
	m_CurrentShader->SetVec3("AmbientColor", ambientColor);*/
	m_ambient_u->BindBuffer();
	m_ambient_u->SetBufferSubData(0, 12, ambientColor.GetPTR());
	m_ambient_u->SetBufferSubData(12, 4, &ambientStrength);
	m_ambient_u->UnbindBuffer();
}

int RaindropRenderer::AppendLight(RD_PointLight* ptLight) {
	m_pt_lights.push_back(ptLight);

	const int lightIndex = (int) std::size(m_pt_lights);

	UpdatePointsLighting();

	return lightIndex;
}

int RaindropRenderer::AppendDirLight(RD_DirLight* dirLight) {
	m_DirLights.push_back(dirLight);

	UpdateDirLighting();
	return 1;
}

void RaindropRenderer::UpdateDirLighting() {
	if (!IsFeatureEnabled(RendererFeature::Lighting))
		return;

	m_dirLights_u->BindBuffer();

	int nbrLights = m_DirLights.size();
	m_dirLights_u->SetBufferSubData(0, sizeof(int), &nbrLights);

	int offset = 16;
	for (int i = 0; i < m_DirLights.size(); i++) {
		float b = m_DirLights[i]->GetBrightness();
		m_dirLights_u->SetBufferSubData(offset, 3 * sizeof(float), m_DirLights[i]->GetLightDir().GetPTR());
		offset += 16;
		m_dirLights_u->SetBufferSubData(offset, 3 * sizeof(float), m_DirLights[i]->GetLightColor().GetPTR());
		offset += 12;
		m_dirLights_u->SetBufferSubData(offset, sizeof(float), &b);
		offset += 4;
	}
	m_dirLights_u->UnbindBuffer();
}

void RaindropRenderer::SwitchShader(RD_ShaderLoader* shader) {
	m_CurrentShader = shader;
	m_CurrentShader->useShader();
}

void RaindropRenderer::RenderDbg(RD_Camera* cam) {
	if constexpr (RENDER_DEBUG_ENABLED) {
		bool rEnableLighting = true;

		if (IsFeatureEnabled(RendererFeature::Lighting)) {
			DisableFeature(RendererFeature::Lighting);
		}
		else {
			rEnableLighting = false;
		}

		m_gbuffer->DebugMode();

		m_api->SetFilledMode(FillingMode::WIREFRAME);

		for (auto* plight : m_pt_lights) {
			m_DBG_light_mdl->GetMaterial()->GetShader()->useShader();

			m_DBG_light_mdl->SetPosition(plight->GetPosition());
			m_DBG_light_mdl->render(cam);
		}

		m_api->SetFilledMode(FillingMode::FILLED);

		if(rEnableLighting)
			EnableFeature(RendererFeature::Lighting);
	}
}

void RaindropRenderer::UpdatePointsLighting() {
	if (!IsFeatureEnabled(RendererFeature::Lighting))
		return;

	m_pointLight_u->BindBuffer();

	int nbrPointLight = m_pt_lights.size();
	m_pointLight_u->SetBufferSubData(0, sizeof(int), &nbrPointLight);

	int offset = 16;
	for (int i = 0; i < m_pt_lights.size(); i++) {
		//FillPtLightIndice(i);
		m_pointLight_u->SetBufferSubData(offset, 3 * sizeof(float), m_pt_lights[i]->GetPosition().GetPTR());
		offset += 16;
		m_pointLight_u->SetBufferSubData(offset, 3 * sizeof(float), m_pt_lights[i]->GetColor().GetPTR());
		offset += 12;
		float b = m_pt_lights[i]->GetBrightness();
		m_pointLight_u->SetBufferSubData(offset, sizeof(float), &b);
		offset += 4;
		float r = m_pt_lights[i]->GetLightRadius();
		m_pointLight_u->SetBufferSubData(offset, sizeof(float), &r);
		offset += 16;
	}

	m_pointLight_u->UnbindBuffer();
}

void RaindropRenderer::FillFeaturesArray() {
	m_renderer_feature[0] = std::pair<std::string, bool>("ftr_specular", true);
	m_renderer_feature[1] = std::pair<std::string, bool>("ftr_lighting", true);
	m_renderer_feature[2] = std::pair<std::string, bool>("ftr_ambient", true);
	m_renderer_feature[3] = std::pair<std::string, bool>("ftr_ssao", true);
	m_renderer_feature[4] = std::pair<std::string, bool>("ftr_bloom", true);
}

void RaindropRenderer::EnableAllFeatures() {
	for (int ftr = RendererFeature::Lighting; ftr != RendererFeature::Ambient; ftr += 1) {
		RendererFeature enb_ftr = static_cast<RendererFeature>(ftr);
		EnableFeature(enb_ftr);
	}
}

//Disable or Enable features
void RaindropRenderer::EnableFeature(RendererFeature ftr) {
	if (IsFeatureEnabled(ftr) == false) {
		//m_CurrentShader->SetBool(m_renderer_feature[ftr].first, true);
		m_renderer_feature[ftr].second = true;
	}
	else {
		std::cerr << "Feature " << m_renderer_feature[ftr].first << " already enabled." << std::endl;
	}
}

void RaindropRenderer::DisableFeature(RendererFeature ftr) {
	if (IsFeatureEnabled(ftr) == true) {
		m_renderer_feature[ftr].second = false;
	}
	else {
		std::cerr << "Feature " << m_renderer_feature[ftr].first << " already disabled." << std::endl;
	}
}

bool RaindropRenderer::IsFeatureEnabled(RendererFeature ftr) {
	return m_renderer_feature[ftr].second;
}

void RaindropRenderer::SendFeatureToShader(RD_ShaderLoader* shader, RendererFeature feature) {
	const int index = feature;
	assert(index < m_renderer_feature.size() && "[RaindropRenderer::SendFeatureToShader] Bad index.");
	
	shader->SetBool(m_renderer_feature[index].first, m_renderer_feature[index].second);
}

float RaindropRenderer::GetFramerate() const {
	return (1.0f / (float)m_frmLmt->GetElapsedTime());
}

RD_ShaderLoader* RaindropRenderer::GetCurrentShader() const {
	return m_CurrentShader;
}

double RaindropRenderer::GetLastDeltaTime() const {
	return m_frmLmt->GetLastDeltaTime();
}

int RaindropRenderer::GetFrameLimit() const {
	return m_frmLmt->GetFrameLimit();
}

void RaindropRenderer::RenderLightsDepth(const vec3f& camPos) {
	if (!IsFeatureEnabled(RendererFeature::Lighting))
		return;

	for (auto* dlight : m_DirLights) {
		dlight->DepthRender(this, camPos);
	}
}

void RaindropRenderer::RegisterMesh(RD_Mesh* mesh) {
	std::cout << "Added mesh to renderer." << std::endl;
	m_meshes.push_back(mesh);
}

void RaindropRenderer::RenderMeshes(RD_Camera* cam) {
	for (auto* mesh : m_meshes) {
		RD_ShaderLoader* shader = mesh->GetMaterial()->GetShader();
		shader->useShader();

		mesh->render(cam);
	}
}

void RaindropRenderer::RenderShadowMeshes() {
	if (!IsFeatureEnabled(RendererFeature::Lighting)) {
		return;
	}

	for (auto* mesh : m_meshes) {
		mesh->renderShadows(m_shadowShader);
	}
}

RD_ShaderLoader* RaindropRenderer::GetShadowShader() const {
	return m_shadowShader;
}

bool RaindropRenderer::CreateGbuff() {
	const int width = GetViewportSize().getX();
    const int height = GetViewportSize().getY();

	m_gbuffer = m_api->CreateFrameBuffer(width, height, false);

	//Position buff
	m_gbuffer->AddAttachement(IMGFORMAT_RGB16F);
	m_g_buffer.gPos = 0;

	//Normal buff
	m_gbuffer->AddAttachement(IMGFORMAT_RGB16F);
	m_g_buffer.gNorm = 1;

	//Albedo buff
	m_gbuffer->AddAttachement(IMGFORMAT_RGBA);
	m_g_buffer.gAlbedo = 2;

	//Specular buff
	m_gbuffer->AddAttachement(IMGFORMAT_R16F);
	m_g_buffer.gSpec = 3;
	
	//Depth
	m_gbuffer->AddAttachement(IMGFORMAT_DEPTH);
	m_g_buffer.gDepth = 4;

	m_gbuffer->BuildFBO();

	m_light_pprocess = m_api->CreateFrameBuffer(width, height, true);
	//Light & PostProcess screen
	m_light_pprocess->AddAttachement(IMGFORMAT_RGB);

	m_light_pprocess->BuildFBO();

	return true;
}

bool RaindropRenderer::CreateGbuff_PBR() {
	const int width = GetViewportSize().getX();
	const int height = GetViewportSize().getY();

	m_gbuffer = m_api->CreateFrameBuffer(width, height, false);

	//Position buff
	m_gbuffer->AddAttachement(IMGFORMAT_RGB16F);
	m_g_buffer.gPos = 0;

	//Normal buff
	m_gbuffer->AddAttachement(IMGFORMAT_RGB16F);
	m_g_buffer.gNorm = 1;

	//Albedo buff
	m_gbuffer->AddAttachement(IMGFORMAT_RGBA);
	m_g_buffer.gAlbedo = 2;

	//Specular buff
	m_gbuffer->AddAttachement(IMGFORMAT_R16F);
	m_g_buffer.gSpec = 3;

	//Metallic/Roughness/AO
	m_gbuffer->AddAttachement(IMGFORMAT_RGB16F);
	m_g_buffer.gMetRoughAO = 4;

	//Emissive
	m_gbuffer->AddAttachement(IMGFORMAT_RGB16F);
	m_g_buffer.gEmissive = 5;
	
	//Depth
	m_gbuffer->AddAttachement(IMGFORMAT_DEPTH, SCALEMODE_LINEAR);
	m_g_buffer.gDepth = 6;

	m_gbuffer->BuildFBO();

	m_light_pprocess = m_api->CreateFrameBuffer(width, height, true);
	//Light & PostProcess screen
	m_light_pprocess->AddAttachement(IMGFORMAT_RGB16F);
	//SSR Texture
	m_light_pprocess->AddAttachement(IMGFORMAT_RGB);
	m_light_pprocess->BuildFBO();
	
	m_ssao_buffer = m_api->CreateFrameBuffer(width, height, true);
	//SSAO Texture
	m_ssao_buffer->AddAttachement(IMGFORMAT_R16F);
	//SSAO-Blur Texture
	m_ssao_buffer->AddAttachement(IMGFORMAT_R16F);
	m_ssao_buffer->BuildFBO();

	m_bloom_buffera = m_api->CreateFrameBuffer(width, height, true);
	m_bloom_buffera->AddAttachement(IMGFORMAT_RGB16F, 1, WRAPMODE_CLAMP2EDGE); //Bloom blur 1
	m_bloom_buffera->BuildFBO();

	m_bloom_bufferb = m_api->CreateFrameBuffer(width, height, true);
	m_bloom_bufferb->AddAttachement(IMGFORMAT_RGB16F, 1, WRAPMODE_CLAMP2EDGE); //Bloom blur 2
	m_bloom_bufferb->BuildFBO();
	
	return true;
}

void RaindropRenderer::RenderGbuff(RD_Camera* cam) {
	assert(cam != nullptr && "[RaindropRenderer::RenderGbuff] Cam was null");

	m_gbuffer->BindFBO();
	m_api->Clear(COLOR_BUFFER | DEPTH_BUFFER);

	RenderMeshes(cam);

	m_gbuffer->UnbindFBO();

	RenderShadows();

	if(m_pipeline == Pipeline::PBR_ENGINE) {
		m_ssao_buffer->BindFBO();

		RenderSSAO(cam);
		
		m_ssao_buffer->UnbindFBO();

		m_light_pprocess->BindFBO();
		m_api->Clear(COLOR_BUFFER);
		
		RenderLightPass(cam->GetLocation());
		RenderSSR(cam);

		RenderBloom();
	} else {
		m_light_pprocess->BindFBO();
		m_api->Clear(COLOR_BUFFER);
		
		RenderLightPass(cam->GetLocation());
	}
	
	RenderPostProcess();

	m_light_pprocess->UnbindFBO();
}

void RaindropRenderer::RenderShadows() {
	SwitchShader(m_shadowCalc);

	m_shadows_buffer->BindFBO();
	
	int texID = 0;
	int i = 0;
	for(auto* dlight : m_DirLights) {
		if (!dlight->GetShadowCasting()) {
			continue;
		}

		dlight->GetDepthTexID()->BindTexture(texID);

		m_shadowCalc->SetInt("ShadowMap[" + std::to_string(i) + "]", texID);
		m_shadowCalc->SetMatrix("lspaceMat[" + std::to_string(i) + "]", dlight->GetLightSpace());

		texID++;
		i++;
	}

	m_shadowCalc->SetInt("NbrDirLights", i);

	m_gbuffer->GetAttachementByIndex(m_g_buffer.gPos)->BindTexture(texID);
	m_shadowCalc->SetInt("gPos", texID);

	m_quad->RenderQuad();
	
	m_shadows_buffer->UnbindFBO();
}

void RaindropRenderer::RenderPostProcess() {
	for (auto* pp : m_pp_effects) {
		pp->RenderEffect(m_light_pprocess->GetAttachementByIndex(0));
	}
}

void RaindropRenderer::RenderLightPass(const vec3f& CamPos) {
	SwitchShader(m_light_shader);
	
	SendFeatureToShader(m_light_shader, RendererFeature::Ambient);
	SendFeatureToShader(m_light_shader, RendererFeature::Specular);
	SendFeatureToShader(m_light_shader, RendererFeature::Lighting);

	m_gbuffer->GetAttachementByIndex(m_g_buffer.gAlbedo)->BindTexture(0);
	m_light_shader->SetInt("gAlbedo", 0);

	m_gbuffer->GetAttachementByIndex(m_g_buffer.gNorm)->BindTexture(1);
	m_light_shader->SetInt("gNormal", 1);

	m_gbuffer->GetAttachementByIndex(m_g_buffer.gPos)->BindTexture(2);
	m_light_shader->SetInt("gPos", 2);

	m_gbuffer->GetAttachementByIndex(m_g_buffer.gSpec)->BindTexture(3);
	m_light_shader->SetInt("gSpec", 3);

	m_shadows_buffer->GetAttachementByIndex(0)->BindTexture(4);
	m_light_shader->SetInt("ShadowPass", 4);

	if (m_pipeline == Pipeline::PBR_ENGINE) {
		m_gbuffer->GetAttachementByIndex(m_g_buffer.gMetRoughAO)->BindTexture(5);
		m_light_shader->SetInt("gMetRoughAO", 5);

		m_ssao_buffer->GetAttachementByIndex(1)->BindTexture(6); //Blurred SSAO pass
		m_light_shader->SetInt("ssao", 6);

		m_gbuffer->GetAttachementByIndex(m_g_buffer.gEmissive)->BindTexture(7);
		m_light_shader->SetInt("gEmissive", 7);
	}

	m_quad->RenderQuad();
}

void RaindropRenderer::RenderBloom() {
	if (!IsFeatureEnabled(RendererFeature::Bloom)) {
		m_bloom_bufferb->BindFBO();
		m_api->Clear(COLOR_BUFFER);
		m_bloom_bufferb->UnbindFBO();
		return;
	}

	SwitchShader(m_bloom);

	for (int i = 0; i < 5; i++) {
		m_bloom_buffera->BindFBO();
		if (i == 0) {
			m_gbuffer->GetAttachementByIndex(m_g_buffer.gEmissive)->BindTexture(0);
			m_bloom->SetInt("threshold", 1);
		} else {
			m_bloom_bufferb->GetAttachementByIndex(0)->BindTexture(0);
			m_bloom->SetInt("threshold", 0);
		}
		m_bloom->SetInt("gShaded", 0);
		m_bloom->SetBool("horizontal", true);

		m_quad->RenderQuad();

		m_bloom_buffera->UnbindFBO();
		m_bloom_bufferb->BindFBO();

		m_bloom->SetInt("threshold", 0);

		m_bloom_buffera->GetAttachementByIndex(0)->BindTexture(0);
		m_bloom->SetInt("gShaded", 0);
		m_bloom->SetInt("horizontal", false);

		m_quad->RenderQuad();

		m_bloom_bufferb->UnbindFBO();
	}
}


void RaindropRenderer::RenderSSR(RD_Camera* cam) {
	SwitchShader(m_ssr_shader);

	//m_gbuffer->GetAttachementByIndex(m_g_buffer.gPos)->BindTexture(0);
	//m_ssr_shader->SetInt("gPos", 0);

	//m_gbuffer->GetAttachementByIndex(m_g_buffer.gNorm)->BindTexture(1);
	//m_ssr_shader->SetInt("gNorm", 1);

	//m_light_pprocess->GetAttachementByIndex(0)->BindTexture(2); //Attachement 0 corresponds to the Light Texture Attachement
	//m_ssr_shader->SetInt("ShadedImg", 2);

	//m_gbuffer->GetAttachementByIndex(m_g_buffer.gMetRoughAO)->BindTexture(3);
	//m_ssr_shader->SetInt("gMetRoughAO", 3);

	//m_gbuffer->GetAttachementByIndex(m_g_buffer.gDepth)->BindTexture(4);
	//m_ssr_shader->SetInt("Depth", 4);

	m_quad->RenderQuad();
}

void RaindropRenderer::RenderSSAO(RD_Camera* cam) {
	if (IsFeatureEnabled(RendererFeature::SSAO)) {
		//Render SSAO
		SwitchShader(m_ssao_shader);

			m_ssao_shader->SetInt("scr_w", GetViewportSize().getX());
			m_ssao_shader->SetInt("scr_h", GetViewportSize().getY());

			m_gbuffer->GetAttachementByIndex(m_g_buffer.gPos)->BindTexture(0);
			m_ssao_shader->SetInt("gPos", 0);

			m_gbuffer->GetAttachementByIndex(m_g_buffer.gNorm)->BindTexture(1);
			m_ssao_shader->SetInt("gNorm", 1);

		m_ssao_noise_tex->BindTexture(2);
		m_ssao_shader->SetInt("noise", 2);

		m_quad->RenderQuad();

		//Blur SSAO
		SwitchShader(m_ssao_blur_shader);

		m_ssao_buffer->GetAttachementByIndex(0)->BindTexture(0);
		m_ssao_blur_shader->SetInt("ssao_tex", 0);

		m_quad->RenderQuad();
	} else {
		m_api->SetClearColor(vec3f(1.0f, 1.0f, 1.0f));
		m_api->Clear(COLOR_BUFFER);
		m_api->SetClearColor(vec3f());
	}
}

void RaindropRenderer::GenerateSSAOKernels() {
	std::uniform_real_distribution<float> randFloat(0.0, 1.0);
	std::uniform_real_distribution<float> randFloat2(-1.0, 1.0);
	std::default_random_engine generator;

	for(int i = 0; i < 64; i++) {
		vec3f sample(
			randFloat2(generator),
			randFloat2(generator),
			randFloat(generator)
		);

		sample.NormalizeVector();
		sample = sample * randFloat(generator);

		float scale = (float)i / 64.0;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample = sample * scale;
		
		m_ssao_kernels.push_back(sample);
	}
	m_ssao_u->BindBuffer();

	int w = GetViewportSize().getX();
	int h = GetViewportSize().getY();
	m_ssao_u->SetBufferSubData(0, sizeof(int), &w);
	m_ssao_u->SetBufferSubData(4, sizeof(int), &h);

	int offset = 16;
	for (auto kern : m_ssao_kernels) {
		m_ssao_u->SetBufferSubData(offset, sizeof(float), &kern);
		offset += 4;
	}

	m_ssao_u->UnbindBuffer();
}

void RaindropRenderer::GenerateSSAONoise() {
	std::vector<float> ssao_noise;

	std::uniform_real_distribution<float> randFloat(-1.0, 1.0);
	std::default_random_engine generator;
	
	for(int i=  0; i < 16; i++) {
		float x = randFloat(generator);
		float y = randFloat(generator);
		float z = randFloat(generator);

		ssao_noise.push_back(x);
		ssao_noise.push_back(y);
		ssao_noise.push_back(z);
	}

	m_ssao_noise_tex = m_api->CreateTexture();
	m_ssao_noise_tex->CreateTextureFromPixels(&ssao_noise[0], 4, 4, IMGFORMAT_RGBA16F);
}


void RaindropRenderer::RenderBeauty() {
	m_api->Clear(COLOR_BUFFER);

	SwitchShader(m_beauty_shader);

	m_light_pprocess->GetAttachementByIndex(0)->BindTexture(5);
	m_beauty_shader->SetInt("lightpass", 5);

	m_gui_manager->GetScreenTexture()->BindTexture(6);
	m_beauty_shader->SetInt("GUIscreen", 6);

	if(m_pipeline == Pipeline::PBR_ENGINE) {
		m_light_pprocess->GetAttachementByIndex(1)->BindTexture(7); //SSR Attachement
		m_beauty_shader->SetInt("SSR", 7);


		m_bloom_buffera->GetAttachementByIndex(0)->BindTexture(8);
		m_beauty_shader->SetInt("bloom", 8);
	}

	m_quad->RenderQuad();
}

void RaindropRenderer::UnregisterMesh(RD_Mesh* mesh) {
	const int index = GetElemIndex<RD_Mesh*>(m_meshes, mesh);

	if (index != -1) {
		m_meshes.erase(m_meshes.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void RaindropRenderer::UnregisterDirLight(RD_DirLight* light) {
	const int index = GetElemIndex<RD_DirLight*>(m_DirLights, light);

	if (index != -1) {
		light->Cleanup(this);
		m_DirLights.erase(m_DirLights.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void RaindropRenderer::UnregisterPointLight(RD_PointLight* ptLight) {
	const int index = GetElemIndex<RD_PointLight*>(m_pt_lights, ptLight);

	if (index != -1) {
		m_pt_lights.erase(m_pt_lights.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void RaindropRenderer::UnregisterAllMeshes() {
	for (auto* msh : m_meshes) {
		std::cout << "Unregistering Mesh" << std::endl;

		delete msh;
	}

	m_meshes.clear();
}

void RaindropRenderer::UnregisterAllPointLights() {
	for (auto* plight : m_pt_lights) {
		std::cout << "Unregistering Point Light" << std::endl;

		delete plight;
	}

	m_pt_lights.clear();
}

void RaindropRenderer::UnregisterAllDirLights() {
	for (auto* dlight : m_DirLights) {
		std::cout << "Unregistering Dir Light" << std::endl;

		delete dlight;
	}

	m_DirLights.clear();
}

RD_Texture* RaindropRenderer::GetBlankTexture() const {
	return m_blankTexture;
}

void RaindropRenderer::DeleteGbuff() {
	//Deleting Gbuff FBO texture & Render buffer
	
}

void RaindropRenderer::RecreateGbuff() const {
	int w = m_api->GetWindowingSystem()->GetWidth();
	int h = m_api->GetWindowingSystem()->GetHeight();

	m_gbuffer->ChangeFramebufferSize(w, h);
	m_light_pprocess->ChangeFramebufferSize(w, h);
}

void RaindropRenderer::SetFullscreenMode(const bool mode) const {
	m_api->GetWindowingSystem()->SetFullscreenMode(mode);
}

void RaindropRenderer::AddToTextureGarbageCollector(unsigned int texID) {
	m_textureGarbageCollector.push_back(texID);
}

void RaindropRenderer::EmptyTextureGarbageCollector() {
	/*for (auto tex : m_textureGarbageCollector) {
		glDeleteTextures(1, &tex);
	}*/
}

void RaindropRenderer::AddToFramebufferGarbageCollector(unsigned int fboID) {
	m_framebufferGarbageCollector.push_back(fboID);
}

void RaindropRenderer::EmptyFramebufferGarbageCollector() {
	/*for (auto fbo : m_framebufferGarbageCollector) {
		glDeleteFramebuffers(1, &fbo);
	}*/
}

RD_ShaderMaterial* RaindropRenderer::FetchShaderFromFile(const std::string& ref) const {
	if (!std::filesystem::exists(ref)) {
		std::cerr << "Shader file " << ref << " does not exist." << std::endl;
		dispErrorMessageBox(StrToWStr("Shader file " + ref + " does not exists"));
		return nullptr;
	}

	if (m_matlib->DoMaterialExists(ref)) {
		return m_matlib->GetMaterialByName(ref);
	}

	BD_MatCustomShaderRead mread(ref);
	const std::string fcode = mread.GetShaderCode();
	const std::string vcode = getFileData(m_engineDir + "/Shaders/glsl/Gshad.vert");

	RD_ShaderLoader* shader = m_api->CreateShader();
	shader->CompileShaderFromCode(vcode, fcode);

	RD_ShaderMaterial* shdmat = new RD_ShaderMaterial(shader);
	for (int i = 0; i < mread.GetTextureCount(); i++) {
		RD_Texture* tex = m_api->CreateTexture();
		tex->LoadTexture(mread.GetTexturePath(i));

		shdmat->AddTexture(mread.GetTextureParamName(i), tex);
	}

	m_matlib->AddMaterialToLib(shdmat, ref);
	return shdmat;
}

void RaindropRenderer::SetErrorFlag(bool val) {
	m_error_flag = val;
}

bool RaindropRenderer::GetErrorFlag() const {
	return m_error_flag;
}

std::string RaindropRenderer::GetEngineDir() const {
	return m_engineDir;
}

void RaindropRenderer::RenderGUI_Screen() const {
	m_gui_manager->RenderScreen();
}

RD_GUI_Manager* RaindropRenderer::GetGUI_Manager() const {
	return m_gui_manager.get();
}

void RaindropRenderer::AddPostProcessEffect(RD_PostProcessEffect* effect) {
	std::cout << "Added new post process effect" << std::endl;
	m_pp_effects.push_back(effect);
}

void RaindropRenderer::RemovePostProcessEffect(RD_PostProcessEffect* effect) {
	const int index = GetElemIndex<RD_PostProcessEffect*>(m_pp_effects, effect);

	if (index != -1) {
		std::cout << "Removed post process effect." << std::endl;

		m_pp_effects.erase(m_pp_effects.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

RD_MaterialLibrary* RaindropRenderer::GetMaterialLibrary() const {
	return m_matlib.get();
}

void RaindropRenderer::ResizeViewport(vec2f pos, vec2f size) {
	const int sx = size.getX();
	const int sy = size.getY();
	
	m_api->SetViewportSize(sx, sy, pos.getX(), pos.getY());

	m_gbuffer->ChangeFramebufferSize(sx, sy);
	m_light_pprocess->ChangeFramebufferSize(sx, sy);
	m_ssao_buffer->ChangeFramebufferSize(sx, sy);
	m_shadows_buffer->ChangeFramebufferSize(sx, sy);
	m_bloom_buffera->ChangeFramebufferSize(sx, sy);
	m_bloom_bufferb->ChangeFramebufferSize(sx, sy);

	m_vp_pos = pos;
	m_vp_size = size;

	m_resize_override = true;
}

void RaindropRenderer::DisableResizeOverride() {
	m_resize_override = false;
}

bool RaindropRenderer::GetResizeOverrideState() const {
	return m_resize_override;
}

vec2f RaindropRenderer::GetViewportSize() const {
	return m_vp_size;
}

vec2f RaindropRenderer::GetViewportPos() const {
	return m_vp_pos;
}

void RaindropRenderer::SetVSync(const bool vsync) {
	m_api->GetWindowingSystem()->SetVSync(vsync);

	m_vsync = vsync;
}

bool RaindropRenderer::IsVSyncActivated() const {
	return m_vsync;
}
