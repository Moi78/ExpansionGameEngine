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
#include "RD_Cubemap.h"
#include "RD_RenderingPipeline.h"

#include "RD_RenderingAPI.h"
#include "RD_RenderingAPI_GL.h"

RaindropRenderer::RaindropRenderer(int w, int h, std::string windowName, API api, RD_RenderingPipeline* pline, int maxFramerate, bool minInit, std::string engineDir) : m_vp_size(w, h), m_vp_pos(0.0f, 0.0f) {
	FillFeaturesArray();

	m_vsync = true;
	
	m_engineDir = std::move(engineDir);

	m_pipeline = pline;

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
	
	m_model_u = m_api->CreateUniformBuffer(16 * sizeof(float), 13);
	m_lightview_u = m_api->CreateUniformBuffer(2 * 16 * sizeof(float), 14);
	m_lightspace_u = m_api->CreateUniformBuffer(10 * 16 * sizeof(float), 15);
	m_lightcount_u = m_api->CreateUniformBuffer(sizeof(int), 17);
	m_text_color_u = m_api->CreateUniformBuffer(3 * sizeof(float), 19);
	m_ambient_u = m_api->CreateUniformBuffer(4 * sizeof(float), 2);
	m_dirLightData_u = m_api->CreateUniformBuffer(sizeof(int) + 10 * sizeof(GLSLDirLight), 4);
	m_pointLightData_u = m_api->CreateUniformBuffer(sizeof(int) + 243 * sizeof(GLSLPointLight), 3);
	m_quadzone_u = m_api->CreateUniformBuffer(sizeof(GLSL_QuadZone), 20);

	GLSL_QuadZone init_q = { {0, 0}, {1, 1} };
	m_quadzone_u->BindBuffer();
	m_quadzone_u->SetBufferSubData(0, sizeof(GLSL_QuadZone), (void*)&init_q);
	m_quadzone_u->UnbindBuffer();

	m_camera_matrix_u = m_api->CreateUniformBuffer(129, 0);
	m_camera_location_u = m_api->CreateUniformBuffer(4 * sizeof(float), 5);
	
	m_defTex = m_api->CreateTexture();
	m_defTex->GenerateColorTex(vec3f(1.0f, 0.0f, 1.0f));

	m_blankTexture = m_api->CreateTexture();
	m_blankTexture->GenerateColorTex(vec3f(1.0f, 1.0f, 1.0f));

	m_current_shader_storage_index = 30;
	m_current_selector = 0;

	m_env_cmap = m_api->CreateCubemap();

	RD_ShaderLoader* shad = m_api->CreateShader();
	shad->compileShaderFromFile(
		m_engineDir + "/Shaders" + folder + "Debug." + vertEXT + "",
		m_engineDir + "/Shaders" + folder + "Debug." + fragEXT + ""
	);

	ambientColor = vec3f(1.0f, 1.0f, 1.0f);

	m_quad = std::make_unique<RD_Quad>(m_api.get());
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

	m_DBG_light_mdl = std::make_unique<RD_Mesh>(this, nullptr, vec3f(), vec3f(), vec3f(0.1f, 0.1f, 0.1f), true);
	m_DBG_light_mdl->loadMesh(m_engineDir + "/Meshes/Light.msh");

	pline->InitPipeline(m_api.get(), m_engineDir);
}

RaindropRenderer::~RaindropRenderer() {
	m_matlib->ClearLibrary();

	m_pt_lights.clear();
	m_DirLights.clear();
	m_meshes.clear();
	
	//Deleting textures
	delete m_defTex;
	delete m_blankTexture;

	//Deleting uniform buffers
	delete m_model_u;
	delete m_lightview_u;
	delete m_lightspace_u;
	delete m_lightcount_u;
	delete m_text_color_u;
	delete m_camera_location_u;
	delete m_camera_matrix_u;
	delete m_pointLightData_u;

	delete m_env_cmap;

	delete m_pipeline;
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

int RaindropRenderer::GetWindowHeigh() const {
	return m_api->GetWindowingSystem()->GetHeight();
}

int RaindropRenderer::GetWindowWidth() const {
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

	int i = 0;
	int sc_l = 0; // Number of shadow sasting lights
	for (auto dl : m_DirLights) {
		dl->SendOnBuffer(m_dirLightData_u, i, 16);
		i++;

		sc_l += dl->GetShadowCasting() ? 1 : 0;
	}

	m_dirLightData_u->BindBuffer();
	m_dirLightData_u->SetBufferSubData(0, sizeof(int), (void*)&i);
	m_dirLightData_u->UnbindBuffer();

	m_lightcount_u->BindBuffer();
	m_lightcount_u->SetBufferSubData(0, sizeof(int), (void*)&sc_l);
	m_lightcount_u->UnbindBuffer();
}

void RaindropRenderer::UpdatePointsLighting() {
	if (!IsFeatureEnabled(RendererFeature::Lighting))
		return;

	// TO REWRITE
	int i = 0;
	for (auto pl : m_pt_lights) {
		pl->SendOnBuffer(m_pointLightData_u, i, 16);
		i++;
	}

	m_pointLightData_u->BindBuffer();
	m_pointLightData_u->SetBufferSubData(0, sizeof(int), (void*)&i);
	m_pointLightData_u->UnbindBuffer();
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

double RaindropRenderer::GetLastDeltaTime() const {
	return m_frmLmt->GetLastDeltaTime();
}

int RaindropRenderer::GetFrameLimit() const {
	return m_frmLmt->GetFrameLimit();
}

void RaindropRenderer::RegisterMesh(RD_Mesh* mesh) {
	std::cout << "Added mesh to renderer." << std::endl;
	m_meshes.push_back(mesh);
}

void RaindropRenderer::RenderParticles() {
	for (auto p : m_partEmitters) {
		p->RenderParticles();
	}
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

void RaindropRenderer::SetFullscreenMode(const bool mode) const {
	m_api->GetWindowingSystem()->SetFullscreenMode(mode);
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
	

}

void RaindropRenderer::RemovePostProcessEffect(RD_PostProcessEffect* effect) {
	/*
	const int index = GetElemIndex<RD_PostProcessEffect*>(m_pp_effects, effect);

	if (index != -1) {
		std::cout << "Removed post process effect." << std::endl;

		m_pp_effects.erase(m_pp_effects.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
	*/
}

void RaindropRenderer::ResizeViewport(vec2f pos, vec2f size) {
	const int sx = size.getX();
	const int sy = size.getY();
	
	m_api->SetViewportSize(sx, sy, pos.getX(), pos.getY());

	m_pipeline->ResizeFramebuffers(m_api.get(), sx, sy);

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

int RaindropRenderer::GetDirLightsCount() {
	return m_DirLights.size();
}

RD_UniformBuffer* RaindropRenderer::GetTextColorUniform() {
	return m_text_color_u;
}

float RaindropRenderer::GetAmbientStrength() {
	return ambientStrength;
}

vec3f RaindropRenderer::GetAmbientColor() {
	return ambientColor;
}

void RaindropRenderer::MakeEnvCubemapFromTexs(std::array<std::string, 6> texs) {
	m_env_cmap->BuildCubemapFromImages(texs);
}

RD_MaterialLibrary* RaindropRenderer::GetMaterialLibrary() {
	return m_matlib.get();
}

void RaindropRenderer::RenderShadows(vec3f camPos) {
	m_pipeline->RenderShadowMaps(m_api.get(), m_DirLights, m_matlib.get(), camPos);
}

void RaindropRenderer::RenderScene() {
	m_pipeline->RenderScene(m_api.get(), m_matlib.get());
}

void RaindropRenderer::PushViewMatrix(mat4f view) {
	m_camera_matrix_u->BindBuffer();
	m_camera_matrix_u->SetBufferSubData(64, 64, view.GetPTR());
	m_camera_matrix_u->UnbindBuffer();
}

void RaindropRenderer::PushProjMatrix(mat4f proj) {
	m_camera_matrix_u->BindBuffer();
	m_camera_matrix_u->SetBufferSubData(0, 64, proj.GetPTR());
	m_camera_matrix_u->UnbindBuffer();
}

void RaindropRenderer::PushCamPos(vec3f pos) {
	m_camera_location_u->BindBuffer();
	m_camera_location_u->SetBufferSubData(0, 12, pos.GetPTR());
	m_camera_location_u->UnbindBuffer();
}

void RaindropRenderer::PushModelMatrix(mat4f mdl) {
	m_model_u->BindBuffer();
	m_model_u->SetBufferSubData(0, 64, (void*)mdl.GetPTR());
	m_model_u->UnbindBuffer();
}

void RaindropRenderer::PushLightProjView(mat4f lightproj, mat4f lightview) {
	m_lightview_u->BindBuffer();
	m_lightview_u->SetBufferSubData(0, 64, lightview.GetPTR());
	m_lightview_u->SetBufferSubData(64, 64, lightproj.GetPTR());
	m_lightview_u->UnbindBuffer();
}

RD_RenderingPipeline* RaindropRenderer::GetRenderingPipeline() const {
	return m_pipeline;
}

void RaindropRenderer::IncrementCurrentSSBOIdx() {
	m_current_shader_storage_index++;
}

int RaindropRenderer::GetCurrentSSBOIdx() {
	return m_current_shader_storage_index;
}

void RaindropRenderer::RenderDebug() {
	RD_ShaderLoader* dbg = m_pipeline->DebugStart();
	m_api->SetViewportSize(
		m_vp_size.getX() * m_vp_scale.getX(), //Scale
		m_vp_size.getY() * m_vp_scale.getY(),
		(m_vp_size.getX() - (m_vp_size.getX() * m_vp_scale.getX())) / 2, // Pos
		(m_vp_size.getY() - (m_vp_size.getY() * m_vp_scale.getY())) / 2);

	m_api->SetFilledMode(FillingMode::WIREFRAME);

	for (auto* plight : m_pt_lights) {
		dbg->useShader();
		dbg->SetVec3("debugColor", plight->GetColor());

		m_DBG_light_mdl->SetPosition(plight->GetPosition());
		m_DBG_light_mdl->render();
	}

	m_api->SetFilledMode(FillingMode::FILLED);

	m_api->SetViewportSize(m_vp_size.getX(), m_vp_size.getX(), 0, 0);
}

void RaindropRenderer::SetQuadzone(vec2f pos, vec2f scale) {
	GLSL_QuadZone init_q = { {pos.getX(), pos.getY()}, {scale.getX(), scale.getY()}};
	m_quadzone_u->BindBuffer();
	m_quadzone_u->SetBufferSubData(0, sizeof(GLSL_QuadZone), (void*)&init_q);
	m_quadzone_u->UnbindBuffer();

	m_vp_scale = scale;
	m_need_cam_updt = true;
}

vec2f RaindropRenderer::GetViewportScale() const {
	return m_vp_scale;
}