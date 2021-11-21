#include "pch.h"
#include "RaindropRenderer.h"

#include "RD_DirLight.h"
#include "RD_Mesh.h"
#include "RD_Quad.h"
#include "RD_Camera.h"
#include "RD_MaterialLibrary.h"
#include "RD_PostProcess.h"
#include "RD_Particles.h"
#include "RD_Materials.h"

#include "RD_RenderingAPI.h"
#include "RD_RenderingAPI_GL.h"

RaindropRenderer::RaindropRenderer(int w, int h, std::string windowName, API api, Pipeline pline, int maxFramerate, bool minInit, std::string engineDir) : m_vp_size(w, h), m_vp_pos(0.0f, 0.0f) {
	FillFeaturesArray();

	m_vsync = true;
	
	m_engineDir = std::move(engineDir);

	if (api == API::OPENGL4) {
		std::cout << "Using OGL4." << std::endl;
		m_api = std::make_unique<RD_RenderingAPI_GL>(this);
	}
	else if (api == API::OPENGL3) {
		std::cout << "Using OGL3." << std::endl;
		m_api = std::make_unique<RD_RenderingAPI_GL>(this, true);
	}

	m_error_flag = false;
	m_resize_override = false;

	assert(m_api != nullptr && "ERROR: No 3D-API selected.");

	m_api->InitializeAPI(w, h, std::move(windowName));

	m_frmLmt = std::make_unique<RD_FrameLimiter>(maxFramerate);
	m_pipeline = pline;

	std::string folder, fragEXT, vertEXT;
	switch (api)
	{
	case OPENGL3:
		folder = "/glsl/gl3/";
		fragEXT = "frag";
		vertEXT = "vert";
		break;
	case OPENGL4:
		folder = "/glsl/gl4/";
		fragEXT = "frag";
		vertEXT = "vert";
		break;
	case VULKAN:
		break;
	case DIRECTX:
		break;
	default:
		break;
	}

	//Shader Compiling
	if (m_pipeline == Pipeline::LAMBERT_ENGINE) {
		std::cout << "Compiling main shaders, lambert shading model..." << std::endl;

		m_shadowShader = m_api->CreateShader();
		m_shadowShader->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "Shadow." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "Shadow." + fragEXT + ""
		);

		m_light_shader = m_api->CreateShader();
		m_light_shader->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "Light." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "Light." + fragEXT + ""
		);

		m_beauty_shader = m_api->CreateShader();
		m_beauty_shader->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "Beauty." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "Beauty." + fragEXT + ""
		);

		m_CurrentShader = m_light_shader;

		ambientStrength = 1.0f;
	}
	else {
		std::cout << "Compiling main shaders, PBR shading model..." << std::endl;

		m_shadowShader = m_api->CreateShader();
		m_shadowShader->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "Shadow." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "Shadow." + fragEXT + "");

		m_light_shader = m_api->CreateShader();
		m_light_shader->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "pbr/Light." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "pbr/Light." + fragEXT + "");

		m_beauty_shader = m_api->CreateShader();
		m_beauty_shader->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "Beauty." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "Beauty." + fragEXT + ""
		);

		m_ssr_shader = m_api->CreateShader();
		m_ssr_shader->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "pbr/SSR." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "pbr/SSR." + fragEXT + ""
		);

		m_ssao_shader = m_api->CreateShader();
		m_ssao_shader->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "pbr/SSAO." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "pbr/SSAO." + fragEXT + ""
		);

		m_ssao_blur_shader = m_api->CreateShader();
		m_ssao_blur_shader->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "pbr/SSAO_Blur." + vertEXT + "", 
			m_engineDir + "/Shaders" + folder + "pbr/SSAO_Blur." + fragEXT + ""
		);

		m_bloom = m_api->CreateShader();
		m_bloom->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "pbr/bloom." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "pbr/bloom." + fragEXT + ""
		);

		m_bloom_apply = m_api->CreateShader();
		m_bloom_apply->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "pbr/BloomApply." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "pbr/BloomApply." + fragEXT + ""
		);
		
		m_CurrentShader = m_light_shader;

		ambientStrength = 0.001f;

		m_ssao_u = m_api->CreateUniformBuffer(64 * sizeof(float), 6);
		m_ssao_tex_handle_s = m_api->CreateShaderStorageBuffer(sizeof(uint64_t), 11);

		GenerateSSAOKernels();
		GenerateSSAONoise();
	}

	m_shadowCalc = m_api->CreateShader();
	m_shadowCalc->compileShaderFromFile(
		m_engineDir + "/Shaders" + folder + "ShadowCalc." + vertEXT + "",
		m_engineDir + "/Shaders" + folder + "ShadowCalc." + fragEXT + ""
	);

	m_shadowBlur = m_api->CreateShader();
	m_shadowBlur->compileShaderFromFile(
		m_engineDir+  "/Shaders" + folder + "ShadowBlur." + vertEXT + "",
		m_engineDir+  "/Shaders" + folder + "ShadowBlur." + fragEXT + ""
	);

	m_gbuff_tex_handles_s = m_api->CreateShaderStorageBuffer(768, 8);
	m_sfx_tex_handles_s = m_api->CreateShaderStorageBuffer(5 * sizeof(uint64_t), 9);
	m_blur_state_s = m_api->CreateShaderStorageBuffer(sizeof(ShaderBlurState), 10);
	m_final_passes_tex_handle_s = m_api->CreateShaderStorageBuffer(5 * sizeof(uint64_t), 12);
	m_shadowmaps_s = m_api->CreateShaderStorageBuffer(10 * sizeof(uint64_t), 16);
	m_glyph_s = m_api->CreateShaderStorageBuffer(sizeof(uint64_t), 18);
	m_final_pass_selector_s = m_api->CreateShaderStorageBuffer(sizeof(int), 15);

	m_pointLight_u = m_api->CreateUniformBuffer(243 * (8 * 4 + sizeof(int)), 3);
	m_dirLights_u = m_api->CreateUniformBuffer(10 * ((7 * 4) + sizeof(int)), 4);
	m_ambient_u = m_api->CreateUniformBuffer(17, 2);
	m_model_u = m_api->CreateUniformBuffer(16 * sizeof(float), 13);
	m_lightview_u = m_api->CreateUniformBuffer(2 * 16 * sizeof(float), 14);
	m_lightspace_u = m_api->CreateUniformBuffer(10 * 16 * sizeof(float), 15);
	m_lightcount_u = m_api->CreateUniformBuffer(sizeof(int), 17);
	m_text_color_u = m_api->CreateUniformBuffer(3 * sizeof(float), 19);

	m_shadows_buffer = m_api->CreateFrameBuffer(GetViewportSize().getX(), GetViewportSize().getY(), true);
	m_shadows_buffer->AddAttachement(IMGFORMAT_RGB);
	m_shadows_buffer->BuildFBO();

	m_shadows_buffer->GetAttachementByIndex(0)->MakeTexBindless(this, m_sfx_tex_handles_s, 0);

	m_shadows_blur = m_api->CreateFrameBuffer(GetViewportSize().getX(), GetViewportSize().getY(), true);
	m_shadows_blur->AddAttachement(IMGFORMAT_RGB);
	m_shadows_blur->BuildFBO();

	m_shadows_blur->GetAttachementByIndex(0)->MakeTexBindless(this, m_sfx_tex_handles_s, 1);

	m_shadows_blur_b = m_api->CreateFrameBuffer(GetViewportSize().getX(), GetViewportSize().getY(), true);
	m_shadows_blur_b->AddAttachement(IMGFORMAT_RGB);
	m_shadows_blur_b->BuildFBO();

	m_shadows_blur_b->GetAttachementByIndex(0)->MakeTexBindless(this, m_sfx_tex_handles_s, 2);
	
	m_defTex = m_api->CreateTexture();
	m_defTex->GenerateColorTex(vec3f(1.0f, 0.0f, 1.0f));

	m_blankTexture = m_api->CreateTexture();
	m_blankTexture->GenerateColorTex(vec3f(1.0f, 1.0f, 1.0f));

	m_current_shader_storage_index = 30;
	m_current_selector = 0;

	m_final_pass_selector_s->BindBuffer();
	m_final_pass_selector_s->SetBufferSubData(0, sizeof(int), (void*)0);
	m_final_pass_selector_s->UnbindBuffer();

	if constexpr (RENDER_DEBUG_ENABLED) {
		RD_ShaderLoader* shad = m_api->CreateShader();
		shad->compileShaderFromFile(
			m_engineDir + "/Shaders" + folder + "Debug." + vertEXT + "",
			m_engineDir + "/Shaders" + folder + "Debug." + fragEXT + ""
		);

		m_dbgMat = new RD_ShaderMaterial(shad, this);

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

	m_quad = std::make_unique<RD_Quad>(this);
	m_quad->Bufferize();

	m_matlib = std::make_unique<RD_MaterialLibrary>();
	m_txtRndrs = std::make_unique<RD_GenericRessourceManager<RD_TextRenderer>>();

	RD_ShaderLoader* ld = m_api->CreateShader();
	ld->compileShaderFromFile(
		m_engineDir + "/Shaders" + folder + "TextRender." + vertEXT + "",
		m_engineDir + "/Shaders" + folder + "TextRender." + fragEXT + ""
	);
	m_matlib->AddMaterialToLib(new RD_ShaderMaterial(ld, this), "text");

	UpdateAmbientLighting();

	m_need_cam_updt = false;
}

RaindropRenderer::~RaindropRenderer() {
	m_matlib->ClearLibrary();

	m_pt_lights.clear();
	m_DirLights.clear();
	m_meshes.clear();

	delete m_gbuffer;
	delete m_light_pprocess;
	delete m_shadows_blur;
	delete m_shadows_blur_b;
	delete m_shadows_buffer;
	delete m_bloom_bufferb;

	//Deleting common shaders
	delete m_shadowShader;
	delete m_light_shader;
	delete m_beauty_shader;
	delete m_dbgMat;
	delete m_shadowCalc;
	delete m_shadowBlur;
	
	//Deleting textures
	delete m_defTex;
	delete m_blankTexture;

	//Deleting uniform buffers
	delete m_pointLight_u;
	delete m_dirLights_u;
	delete m_ambient_u;
	delete m_model_u;
	delete m_lightview_u;
	delete m_lightspace_u;
	delete m_lightcount_u;
	delete m_text_color_u;

	//Deleting shader storage buffers
	delete m_gbuff_tex_handles_s;
	delete m_sfx_tex_handles_s;
	delete m_blur_state_s;
	delete m_final_passes_tex_handle_s;
	delete m_shadowmaps_s;
	delete m_glyph_s;
	delete m_final_pass_selector_s;

	//PBR related deletion
	if (m_pipeline == Pipeline::PBR_ENGINE) {
		delete m_ssao_buffer;

		delete m_ssr_shader;
		delete m_ssao_shader;
		delete m_ssao_blur_shader;
		delete m_bloom;
		delete m_bloom_apply;

		delete m_ssao_noise_tex;
		delete m_ssao_tex_handle_s;
		delete m_bloom_buffera;

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

void RaindropRenderer::UpdateDirLighting(const bool lspace_only) {
	if (!IsFeatureEnabled(RendererFeature::Lighting))
		return;

	if (!lspace_only) {
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

		m_shadowmaps_s->BindBuffer();
		int i = 0;

		for (auto l : m_DirLights) {
			if (!l->GetShadowCasting()) {
				continue;
			}

			uint64_t handle = l->GetDepthTexID()->GetTextureHandle();
			m_shadowmaps_s->SetBufferSubData(i * sizeof(uint64_t), sizeof(uint64_t), (void*)&handle);

			i++;
		}

		m_shadowmaps_s->UnbindBuffer();

		m_lightcount_u->BindBuffer();
		int nbr = i;
		m_lightcount_u->SetBufferSubData(0, sizeof(int), (void*)&nbr);
		m_lightcount_u->UnbindBuffer();
	}

	m_lightspace_u->BindBuffer();

	int offset = 0;
	for (auto l : m_DirLights) {
		m_lightspace_u->SetBufferSubData(offset, 16 * sizeof(float), (void*)l->GetLightSpace().GetPTR());
		offset += 16 * sizeof(float);
	}

	m_lightspace_u->UnbindBuffer();
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
			m_DBG_light_mdl->render();
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
	m_renderer_feature[0] = std::pair<std::string, bool>("ftr_specular", true); // Non-working antiquity
	m_renderer_feature[1] = std::pair<std::string, bool>("ftr_lighting", true); // Non-working antiquity that needs to be fixed
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

	UpdateDirLighting(true);
}

void RaindropRenderer::RegisterMesh(RD_Mesh* mesh) {
	std::cout << "Added mesh to renderer." << std::endl;
	m_meshes.push_back(mesh);
}

void RaindropRenderer::RenderMeshes(RD_Camera* cam) {
	for (auto mat : m_matlib->GetRawVector()) {
		mat.second->DrawMeshes();
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

	for (int i = 0; i < 4; i++) {
		m_gbuffer->GetAttachementByIndex(i)->MakeTexBindless(this, m_gbuff_tex_handles_s, i);
	}

	m_light_pprocess = m_api->CreateFrameBuffer(width, height, true);
	//Light & PostProcess screen
	m_light_pprocess->AddAttachement(IMGFORMAT_RGB);

	m_light_pprocess->BuildFBO();

	m_bloom_bufferb = m_api->CreateFrameBuffer(width, height, true);
	m_bloom_bufferb->AddAttachement(IMGFORMAT_RGB);
	m_bloom_bufferb->BuildFBO();

	return true;
}

bool RaindropRenderer::CreateGbuff_PBR() {
	const int width = GetViewportSize().getX();
	const int height = GetViewportSize().getY();

	m_gbuffer = m_api->CreateFrameBuffer(width, height, false);
	//m_gbuffer->SetMultisampled(true);

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
	
	for (int i = 0; i < 6; i++) {
		m_gbuffer->GetAttachementByIndex(i)->MakeTexBindless(this, m_gbuff_tex_handles_s, i);
	}

	m_light_pprocess = m_api->CreateFrameBuffer(width, height, true);
	//Light & PostProcess screen
	m_light_pprocess->AddAttachement(IMGFORMAT_RGB16F);
	m_light_pprocess->BuildFBO();

	m_light_pprocess->GetAttachementByIndex(0)->MakeTexBindless(this, m_final_passes_tex_handle_s, 0);
	
	m_ssao_buffer = m_api->CreateFrameBuffer(width, height, true);
	//SSAO Texture
	m_ssao_buffer->AddAttachement(IMGFORMAT_R16F);
	//SSAO-Blur Texture
	m_ssao_buffer->AddAttachement(IMGFORMAT_R16F);
	m_ssao_buffer->BuildFBO();

	m_ssao_buffer->GetAttachementByIndex(0)->MakeTexBindless(this, m_sfx_tex_handles_s, 3);
	m_ssao_buffer->GetAttachementByIndex(1)->MakeTexBindless(this, m_sfx_tex_handles_s, 4);

	m_bloom_buffera = m_api->CreateFrameBuffer(width, height, true);
	m_bloom_buffera->AddAttachement(IMGFORMAT_RGB16F, 1, WRAPMODE_CLAMP2EDGE); //Bloom blur 1
	m_bloom_buffera->BuildFBO();

	m_bloom_buffera->GetAttachementByIndex(0)->MakeTexBindless(this, m_final_passes_tex_handle_s, 2);

	m_bloom_bufferb = m_api->CreateFrameBuffer(width, height, true);
	m_bloom_bufferb->AddAttachement(IMGFORMAT_RGB16F, 1, WRAPMODE_CLAMP2EDGE); //Bloom blur 2
	m_bloom_bufferb->BuildFBO();

	m_bloom_bufferb->GetAttachementByIndex(0)->MakeTexBindless(this, m_final_passes_tex_handle_s, 3);
	
	return true;
}

void RaindropRenderer::RenderGbuff(RD_Camera* cam) {
	assert(cam != nullptr && "[RaindropRenderer::RenderGbuff] Cam was null");

	m_gbuffer->BindFBO();
	m_api->Clear(COLOR_BUFFER | DEPTH_BUFFER);

	RenderMeshes(cam);
	RenderParticles();

	m_gbuffer->UnbindFBO();
	if (m_gbuffer->IsFBMultisampled()) {
		m_gbuffer->MultisampledToIntermediate();
	}

	RenderShadows();

	if(m_pipeline == Pipeline::PBR_ENGINE) {
		m_ssao_buffer->BindFBO();

		RenderSSAO();
		
		m_ssao_buffer->UnbindFBO();

		m_light_pprocess->BindFBO();
		m_api->Clear(COLOR_BUFFER);
		
		RenderLightPass(cam->GetLocation());
		RenderSSR();

		RenderBloom();
	} else {
		m_light_pprocess->BindFBO();
		m_api->Clear(COLOR_BUFFER);
		
		RenderLightPass(cam->GetLocation());

		m_bloom_bufferb->BindFBO();
		m_api->Clear(COLOR_BUFFER);
		m_bloom_bufferb->UnbindFBO();
	}

	m_light_pprocess->UnbindFBO();

	RenderPostProcess();
}

void RaindropRenderer::RenderParticles() {
	for (auto p : m_partEmitters) {
		p->RenderParticles();
	}
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

		if (dlight->GetDepthTexID()->BindTexture(texID)) {
			m_shadowCalc->SetInt("ShadowMap[" + std::to_string(i) + "]", texID);
		}

		texID++;
		i++;
	}

	m_shadowCalc->SetInt("NbrDirLights", i);

	if (m_gbuffer->GetAttachementByIndex(m_g_buffer.gPos)->BindTexture(texID)) {
		m_shadowCalc->SetInt("gPos", texID);
	}

	m_quad->RenderQuad();
	
	m_shadows_buffer->UnbindFBO();

	SwitchShader(m_shadowBlur);
	m_shadows_blur->BindFBO();

	constexpr ShaderBlurState st_a{ { 0.0f, 1.0f, 0.0f }, 0};
	constexpr ShaderBlurState st_b{ { 1.0f, 0.0f, 0.0f }, 1};

	if (m_shadows_buffer->GetAttachementByIndex(0)->BindTexture(0)) {
		m_shadowBlur->SetInt("baseImage", 0);
	}

	m_blur_state_s->BindBuffer();
	m_blur_state_s->SetBufferSubData(0, sizeof(ShaderBlurState), (void*)&st_a);
	m_blur_state_s->UnbindBuffer();

	m_quad->RenderQuad();

	m_shadows_blur_b->BindFBO();

	m_shadows_blur->GetAttachementByIndex(0)->BindTexture(0);

	m_blur_state_s->BindBuffer();
	m_blur_state_s->SetBufferSubData(0, sizeof(ShaderBlurState), (void*)&st_b);
	m_blur_state_s->UnbindBuffer();

	m_quad->RenderQuad();

	m_shadows_blur_b->UnbindFBO();
}

void RaindropRenderer::RenderPostProcess() {
	int selector = 0;
	m_final_pass_selector_s->BindBuffer();
	m_final_pass_selector_s->SetBufferSubData(0, sizeof(int), (void*)&selector);
	m_final_pass_selector_s->UnbindBuffer();

	m_light_pprocess->GetAttachementByIndex(0)->BindTexture(0);

	//Recycling bloom buffers
	int i = 0;
	for (auto* pp : m_pp_effects) {
		if (i % 2 == 0) {
			if (i > 0) {
				selector = 3;
				m_final_pass_selector_s->BindBuffer();
				m_final_pass_selector_s->SetBufferSubData(0, sizeof(int), (void*)&selector);
				m_final_pass_selector_s->UnbindBuffer();

				m_bloom_bufferb->GetAttachementByIndex(0)->BindTexture(0);
			}

			m_bloom_buffera->BindFBO();
			pp->RenderEffect(m_light_pprocess->GetAttachementByIndex(0));
			m_bloom_buffera->UnbindFBO();
		}
		else {
			if (i > 0) {
				selector = 2;
				m_final_pass_selector_s->BindBuffer();
				m_final_pass_selector_s->SetBufferSubData(0, sizeof(int), (void*)&selector);
				m_final_pass_selector_s->UnbindBuffer();

				m_bloom_buffera->GetAttachementByIndex(0)->BindTexture(0);
			}

			m_bloom_bufferb->BindFBO();
			pp->RenderEffect(m_light_pprocess->GetAttachementByIndex(0));
			m_bloom_bufferb->UnbindFBO();
		}

		i++;
	}

	if (i > 0) {
		if (i % 2 == 0) {
			selector = 3;
		}
		else {
			selector = 2;
		}
	}
	else {
		selector = 0;
	}

	m_final_pass_selector_s->BindBuffer();
	m_final_pass_selector_s->SetBufferSubData(0, sizeof(int), (void*)&selector);
	m_final_pass_selector_s->UnbindBuffer();

	m_current_selector = 0;
}

void RaindropRenderer::RenderLightPass(const vec3f& CamPos) {
	SwitchShader(m_light_shader);
	
	//SendFeatureToShader(m_light_shader, RendererFeature::Ambient);
	//SendFeatureToShader(m_light_shader, RendererFeature::Specular);
	//SendFeatureToShader(m_light_shader, RendererFeature::Lighting);

	if (m_gbuffer->GetAttachementByIndex(m_g_buffer.gAlbedo)->BindTexture(0)) {
		m_light_shader->SetInt("gAlbedo", 0);
	}

	if (m_gbuffer->GetAttachementByIndex(m_g_buffer.gNorm)->BindTexture(1)) {
		m_light_shader->SetInt("gNormal", 1);
	}

	if (m_gbuffer->GetAttachementByIndex(m_g_buffer.gPos)->BindTexture(2)) {
		m_light_shader->SetInt("gPos", 2);
	}

	if (m_gbuffer->GetAttachementByIndex(m_g_buffer.gSpec)->BindTexture(3)) {
		m_light_shader->SetInt("gSpec", 3);
	}

	if (m_shadows_blur_b->GetAttachementByIndex(0)->BindTexture(4)) {
		m_light_shader->SetInt("ShadowPass", 4);
	}

	if (m_pipeline == Pipeline::PBR_ENGINE) {
		if (m_gbuffer->GetAttachementByIndex(m_g_buffer.gMetRoughAO)->BindTexture(5)) {
			m_light_shader->SetInt("gMetRoughAO", 5);
		}

		if (m_ssao_buffer->GetAttachementByIndex(1)->BindTexture(6)) { //Blurred SSAO pass
			m_light_shader->SetInt("ssao", 6);
		}

		if (m_gbuffer->GetAttachementByIndex(m_g_buffer.gEmissive)->BindTexture(7)) {
			m_light_shader->SetInt("gEmissive", 7);
		}
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

	constexpr ShaderBlurState st_a{ {1.0f, 0.0f}, 5, 1, 1 };
	constexpr ShaderBlurState st_b{ {0.0f, 1.0f}, 2, 0, 0 };
	constexpr ShaderBlurState st_c{ {1.0f, 0.0f}, 3, 0, 0 };

	m_bloom_buffera->BindFBO();
	if (m_gbuffer->GetAttachementByIndex(m_g_buffer.gEmissive)->BindTexture(0)) {
		m_bloom->SetInt("gShaded", 0);
	}

	m_blur_state_s->BindBuffer();
	m_blur_state_s->SetBufferSubData(0, sizeof(ShaderBlurState), (void*)&st_a);
	m_blur_state_s->UnbindBuffer();

	m_quad->RenderQuad();

	m_bloom_bufferb->BindFBO();
	m_bloom_buffera->GetAttachementByIndex(0)->BindTexture(0);

	m_blur_state_s->BindBuffer();
	m_blur_state_s->SetBufferSubData(0, sizeof(ShaderBlurState), (void*)&st_b);
	m_blur_state_s->UnbindBuffer();

	m_quad->RenderQuad();

	m_bloom_buffera->BindFBO();
	m_bloom_bufferb->GetAttachementByIndex(0)->BindTexture(0);

	m_blur_state_s->BindBuffer();
	m_blur_state_s->SetBufferSubData(0, sizeof(ShaderBlurState), (void*)&st_c);
	m_blur_state_s->UnbindBuffer();

	m_quad->RenderQuad();

	//m_api->Clear(COLOR_BUFFER);
	m_light_pprocess->BindFBO();

	SwitchShader(m_bloom_apply);

	if (m_light_pprocess->GetAttachementByIndex(0)->BindTexture(0)) {
		m_bloom_apply->SetInt("screen", 0);
	}

	if (m_bloom_bufferb->GetAttachementByIndex(0)->BindTexture(1)) {
		m_bloom_apply->SetInt("bloom", 1);
	}

	m_quad->RenderQuad();
}


void RaindropRenderer::RenderSSR() {
	SwitchShader(m_ssr_shader);

	m_quad->RenderQuad();
}

void RaindropRenderer::RenderSSAO() {
	if (IsFeatureEnabled(RendererFeature::SSAO)) {
		//Render SSAO
		SwitchShader(m_ssao_shader);

		if (m_gbuffer->GetAttachementByIndex(m_g_buffer.gPos)->BindTexture(0)) {
			m_ssao_shader->SetInt("gPos", 0);
		}

		if (m_gbuffer->GetAttachementByIndex(m_g_buffer.gNorm)->BindTexture(1)) {
			m_ssao_shader->SetInt("gNorm", 1);
		}

		if (m_ssao_noise_tex->BindTexture(2)) {
			m_ssao_shader->SetInt("noise", 2);
		}

		m_quad->RenderQuad();

		//Blur SSAO
		SwitchShader(m_ssao_blur_shader);

		if (m_ssao_buffer->GetAttachementByIndex(0)->BindTexture(0)) {
			m_ssao_blur_shader->SetInt("ssao_tex", 0);
		}

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

	m_ssao_noise_tex->MakeTexBindless(this, m_ssao_tex_handle_s, 0);
}


void RaindropRenderer::RenderBeauty() {
	m_api->Clear(COLOR_BUFFER);

	SwitchShader(m_beauty_shader);

	if (m_current_selector == 0) {
		if (m_light_pprocess->GetAttachementByIndex(0)->BindTexture(5)) {
			m_beauty_shader->SetInt("lightpass", 5);
		}
	}
	else if (m_current_selector == 2) {
		if (m_bloom_buffera->GetAttachementByIndex(0)->BindTexture(5)) {
			m_beauty_shader->SetInt("lightpass", 5);
		}
	}
	else if (m_current_selector == 3) {
		if (m_bloom_bufferb->GetAttachementByIndex(0)->BindTexture(5)) {
			m_beauty_shader->SetInt("lightpass", 5);
		}
	}

	m_quad->RenderQuad();
}

void RaindropRenderer::UnregisterMesh(RD_Mesh* mesh) {
	const int index = GetElemIndex<RD_Mesh*>(m_meshes, mesh);

	if (index != -1) {
		m_meshes.erase(m_meshes.begin() + index);
	}
	else {
		std::cerr << "ERROR: (Mesh) Element does not exists" << std::endl;
	}
}

void RaindropRenderer::UnregisterDirLight(RD_DirLight* light) {
	const int index = GetElemIndex<RD_DirLight*>(m_DirLights, light);

	if (index != -1) {
		light->Cleanup(this);
		m_DirLights.erase(m_DirLights.begin() + index);
	}
	else {
		std::cerr << "ERROR: (Dir Light) Element does not exists" << std::endl;
	}

	UpdateDirLighting();
}

void RaindropRenderer::UnregisterPointLight(RD_PointLight* ptLight) {
	const int index = GetElemIndex<RD_PointLight*>(m_pt_lights, ptLight);

	if (index != -1) {
		m_pt_lights.erase(m_pt_lights.begin() + index);
	}
	else {
		std::cerr << "ERROR: (Point Light) Element does not exists" << std::endl;
	}

	UpdatePointsLighting();
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

RD_ShaderMaterial* RaindropRenderer::FetchShaderFromFile(const std::string& ref, const std::string& texPathPrefix, const bool noreg) {
	if (!std::filesystem::exists(ref)) {
		std::cerr << "Shader file " << ref << " does not exist." << std::endl;
		dispErrorMessageBox(StrToWStr("Shader file " + ref + " does not exists"));
	}

	if ((m_matlib->DoMaterialExists(ref)) && (!noreg)) {
		return m_matlib->GetMaterialByName(ref);
	}

	std::string folder, vertEXT;
	bool legacy = false;
	switch (m_api->GetAPIType())
	{
	case API::OPENGL3:
		folder = "/glsl/gl3/";
		vertEXT = "vert";
		legacy = true;
		break;
	case API::OPENGL4:
		folder = "/glsl/gl4/";
		vertEXT = "vert";
		break;
	default:
		break;
	}

	BD_MatCustomShaderRead mread(ref);
	const std::string fcode = mread.GetShaderCode(legacy);
	const std::string vcode = getFileData(m_engineDir + "/Shaders" + folder + "Gshad." + vertEXT + "");

	RD_ShaderLoader* shader = m_api->CreateShader();
	shader->CompileShaderFromCode(vcode, fcode);

	RD_ShaderMaterial* shdmat = new RD_ShaderMaterial(shader, this);
	for (int i = 0; i < mread.GetTextureCount(); i++) {
		RD_Texture* tex = m_api->CreateTexture();
		tex->LoadTexture(texPathPrefix + mread.GetTexturePath(i));

		shdmat->AddTexture(mread.GetTextureParamName(i), tex);
	}
	shdmat->MakeSSBO();

	if (!noreg) {
		m_matlib->AddMaterialToLib(shdmat, ref);
	}

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

	for (int i = 0; i < 6; i++) {
		m_gbuffer->GetAttachementByIndex(i)->MakeTexBindless(this, m_gbuff_tex_handles_s, i);
	}

	m_light_pprocess->ChangeFramebufferSize(sx, sy);
	m_light_pprocess->GetAttachementByIndex(0)->MakeTexBindless(this, m_final_passes_tex_handle_s, 0);

	m_ssao_buffer->ChangeFramebufferSize(sx, sy);
	m_ssao_buffer->GetAttachementByIndex(0)->MakeTexBindless(this, m_sfx_tex_handles_s, 3);
	m_ssao_buffer->GetAttachementByIndex(1)->MakeTexBindless(this, m_sfx_tex_handles_s, 4);

	m_shadows_buffer->ChangeFramebufferSize(sx, sy);
	m_shadows_buffer->GetAttachementByIndex(0)->MakeTexBindless(this, m_sfx_tex_handles_s, 0);

	m_shadows_blur->ChangeFramebufferSize(sx, sy);
	m_shadows_blur->GetAttachementByIndex(0)->MakeTexBindless(this, m_sfx_tex_handles_s, 1);

	m_shadows_blur_b->ChangeFramebufferSize(sx, sy);
	m_shadows_blur_b->GetAttachementByIndex(0)->MakeTexBindless(this, m_sfx_tex_handles_s, 2);

	m_bloom_buffera->ChangeFramebufferSize(sx, sy);
	m_bloom_buffera->GetAttachementByIndex(0)->MakeTexBindless(this, m_final_passes_tex_handle_s, 2);

	m_bloom_bufferb->ChangeFramebufferSize(sx, sy);
	m_bloom_bufferb->GetAttachementByIndex(0)->MakeTexBindless(this, m_final_passes_tex_handle_s, 3);

	m_vp_pos = pos;
	m_vp_size = size;

	m_resize_override = true;
	m_need_cam_updt = true;
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

RD_GenericRessourceManager<RD_TextRenderer>* RaindropRenderer::GetTxtRendererManager() const {
	return m_txtRndrs.get();
}

void RaindropRenderer::RegisterParticleEmitter(RD_ParticleEmitter* emitter) {
	std::cout << "Registering new particle emitter" << std::endl;
	m_partEmitters.push_back(emitter);
}

void RaindropRenderer::UnregisterParticleEmitter(RD_ParticleEmitter* emitter) {
	std::cout << "Unregistering particle emitter" << std::endl;
	const int index = GetElemIndex<RD_ParticleEmitter*>(m_partEmitters, emitter);

	if (index != -1) {
		m_partEmitters.erase(m_partEmitters.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void RaindropRenderer::UpdateParticles() {
	for (auto p : m_partEmitters) {
		p->UpdateParticles();
	}
}

void RaindropRenderer::UnregisterAllParticleEmitters() {
	for (auto p : m_partEmitters) {
		UnregisterParticleEmitter(p);
		delete p;
	}
}

bool RaindropRenderer::DoNeedCamUpdate() {
	if (m_need_cam_updt) {
		m_need_cam_updt = false;
		return true;
	}

	return false;
}

void RaindropRenderer::PushModelMatrix(mat4f& model) {
	m_model_u->BindBuffer();
	m_model_u->SetBufferSubData(0, 16 * sizeof(float), (void*)model.GetPTR());
	m_model_u->UnbindBuffer();
}

void RaindropRenderer::PushLightProjViewMatrices(mat4f& lview, mat4f& lproj) {
	m_lightview_u->BindBuffer();
	m_lightview_u->SetBufferSubData(0, 16 * sizeof(float), (void*)lview.GetPTR());
	m_lightview_u->SetBufferSubData(16 * sizeof(float), 16 * sizeof(float), (void*)lproj.GetPTR());
	m_lightview_u->UnbindBuffer();
}

RD_ShaderStorageBuffer* RaindropRenderer::GetShadowMapsBufferHandle() {
	return m_shadowmaps_s;
}

int RaindropRenderer::GetDirLightsCount() {
	return m_DirLights.size();
}

RD_ShaderStorageBuffer* RaindropRenderer::GetGlyphTexHandle() {
	return m_glyph_s;
}

RD_UniformBuffer* RaindropRenderer::GetTextColorUniform() {
	return m_text_color_u;
}

int RaindropRenderer::GetCurrentShaderStorageIndex() {
	return m_current_shader_storage_index;
}

void RaindropRenderer::IncrementCurrentShaderStorageIndex() {
	m_current_shader_storage_index++;
}

float RaindropRenderer::GetAmbientStrength() {
	return ambientStrength;
}

vec3f RaindropRenderer::GetAmbientColor() {
	return ambientColor;
}