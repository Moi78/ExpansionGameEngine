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

RaindropRenderer::RaindropRenderer(int w, int h, std::string windowName, API api, Pipeline pline, int maxFramerate, bool minInit, std::string engineDir) {
	FillFeaturesStringArray();
	FillFeatureStateArray();

	m_engineDir = engineDir;

	if (api == API::OPENGL) {
		m_api = std::make_unique<RD_RenderingAPI_GL>(this);
	}

	m_error_flag = false;

	assert(m_api != nullptr && "ERROR: No 3D-API selected.");

	m_api->InitializeAPI(w, h, windowName);

	m_frmLmt = std::make_unique<RD_FrameLimiter>(maxFramerate);
	m_pipeline = pline;

	//Shader Compiling
	if (m_pipeline == Pipeline::LAMBERT_ENGINE) {
		std::cout << "Compiling main shaders, lambert shading model..." << std::endl;

		m_shadowShader = m_api->CreateShader();
		m_shadowShader->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Shadow.vert", m_engineDir + "/Shaders/glsl/Shadow.frag");

		m_light_shader = m_api->CreateShader();
		m_light_shader->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Light.vert", m_engineDir + "/Shaders/glsl/Light.frag");

		m_beauty_shader = m_api->CreateShader();
		m_beauty_shader->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Beauty.vert", m_engineDir + "/Shaders/glsl/Beauty.frag");

		m_CurrentShader = m_light_shader;

		ambientStrength = 1.0f;
	}
	else {
		std::cout << "Compiling main shaders, PBR shading model..." << std::endl;

		m_shadowShader = m_api->CreateShader();
		m_shadowShader->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Shadow.vert", m_engineDir + "/Shaders/glsl/Shadow.frag");

		m_light_shader = m_api->CreateShader();
		m_light_shader->compileShaderFromFile(m_engineDir + "/Shaders/glsl/pbr/Light.vert", m_engineDir + "/Shaders/glsl/pbr/Light.frag");

		m_beauty_shader = m_api->CreateShader();
		m_beauty_shader->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Beauty.vert", m_engineDir + "/Shaders/glsl/Beauty.frag");

		m_CurrentShader = m_light_shader;

		ambientStrength = 0.001f;
	}

	m_defTex = m_api->CreateTexture();
	m_defTex->LoadTexture(m_engineDir + "/Textures/defTex.png");

	m_blankTexture = m_api->CreateTexture();
	m_blankTexture->GenerateColorTex(vec3f(1.0f, 1.0f, 1.0f));

	if (RENDER_DEBUG_ENABLED) {
		RD_ShaderLoader* shad = m_api->CreateShader();
		shad->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Debug.vert", m_engineDir + "/Shaders/glsl/Debug.frag");
		RD_ShaderMaterial* dbgmat = new RD_ShaderMaterial(shad);

		m_DBG_light_mdl = std::make_unique<RD_Mesh>(this, dbgmat, vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.3f, 0.3f, 0.3f));
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
}

RaindropRenderer::~RaindropRenderer() {
	m_pt_lights.clear();
	m_DirLights.clear();
	m_meshes.clear();

	delete m_gbuffer;

	delete m_shadowShader;
	delete m_light_shader;
	delete m_beauty_shader;
}

RD_RenderingAPI* RaindropRenderer::GetRenderingAPI() {
	return m_api.get();
}

void RaindropRenderer::ClearWindow(vec3f refreshColor) {
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

bool RaindropRenderer::WantToClose() {
	if (m_api->GetWindowingSystem()->WantToClose()) {
		return true;
	}
	else {
		return false;
	}
}

int RaindropRenderer::getWindowHeigh() {
	return m_api->GetWindowingSystem()->GetHeight();
}

int RaindropRenderer::getWindowWidth() {
	return m_api->GetWindowingSystem()->GetWidth();
}

void RaindropRenderer::SetAmbientStrength(float strength) {
	ambientStrength = strength;

	UpdateAmbientLighting();
}

void RaindropRenderer::SetAmbientColor(vec3f nColor) {
	ambientColor = nColor;

	UpdateAmbientLighting();
}

void RaindropRenderer::UpdateAmbientLighting() {
	if (!IsFeatureEnabled(RendererFeature::Ambient))
		return;

	m_CurrentShader->SetFloat("AmbientStrength", ambientStrength);
	m_CurrentShader->SetVec3("AmbientColor", ambientColor);
}

int RaindropRenderer::AppendLight(RD_PointLight* ptLight) {
	std::cout << "Registering light color : " << ptLight->GetColor().getX() << " " << ptLight->GetColor().getY() << " " << ptLight->GetColor().getZ() << " brightness : " << ptLight->GetBrightness() << std::endl;

	m_pt_lights.push_back(ptLight);

	int lightIndex = (int) std::size(m_pt_lights);

	//UpdatePointsLighting();

	return lightIndex;
}

int RaindropRenderer::AppendDirLight(RD_DirLight* dirLight) {
	m_DirLights.push_back(dirLight);

	return 1;
}

void RaindropRenderer::UpdateDirLighting() {
	if (!IsFeatureEnabled(RendererFeature::Lighting))
		return;

	for (int i = 0; i < m_DirLights.size(); i++) {
		FillDirLightIndice(i);
	}

	m_CurrentShader->SetInt("nbrDirLight", (int) m_DirLights.size());
}

void RaindropRenderer::FillDirLightIndice(int index) {
	if (!IsFeatureEnabled(RendererFeature::Lighting))
		return;

	if (index > m_DirLights.size()) {
		std::cerr << "Can't add this directionnal light : Index out of range." << std::endl;
		return;
	}
	else if (index > 10) {
		std::cerr << "No more than 10 Directional Lights are supported." << std::endl;
		return;
	}

	std::string indexSTR = std::to_string(index);

	m_CurrentShader->SetVec3("DirLightDir[" + indexSTR + "]", m_DirLights[index]->GetLightDir());
	m_CurrentShader->SetVec3("DirLightColor[" + indexSTR + "]", m_DirLights[index]->GetLightColor());
	m_CurrentShader->SetFloat("DirLightBrightness[" + indexSTR + "]", m_DirLights[index]->GetBrightness());
}

void RaindropRenderer::SwitchShader(RD_ShaderLoader* shader) {
	m_CurrentShader = shader;
	m_CurrentShader->useShader();
}

void RaindropRenderer::RenderDbg(RD_Camera* cam) {
	if (RENDER_DEBUG_ENABLED) {
		bool rEnableLighting = true;

		if (IsFeatureEnabled(RendererFeature::Lighting)) {
			DisableFeature(RendererFeature::Lighting);
		}
		else {
			rEnableLighting = false;
		}

		m_gbuffer->DebugMode();

		m_api->SetFilledMode(FillingMode::WIREFRAME);

		for (int i = 0; i < m_pt_lights.size(); i++) {
			m_DBG_light_mdl->GetMaterial()->GetShader()->useShader();

			m_DBG_light_mdl->SetPosition(m_pt_lights[i]->GetPosition());
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

	for (int i = 0; i < m_pt_lights.size(); i++) {
		FillPtLightIndice(i);
	}

	m_CurrentShader->SetInt("nbrPointLight", (int) m_pt_lights.size());
}

void RaindropRenderer::FillPtLightIndice(int index) {
	if (m_pt_lights.size() < index) {
		std::cerr << "Can't add point light : Index out of range." << std::endl;
	}
	else if (index > 243) {
		std::cerr << "Can't add point light : No more than 243 lights are supported." << std::endl;
	}

	std::string indexSTR = std::to_string(index);

	m_CurrentShader->SetVec3("LightPos["+ indexSTR +"]", m_pt_lights[index]->GetPosition());
	m_CurrentShader->SetFloat("LightBrightness[" + indexSTR + "]", m_pt_lights[index]->GetBrightness());
	m_CurrentShader->SetVec3("LightColor[" + indexSTR + "]", m_pt_lights[index]->GetColor());
	m_CurrentShader->SetFloat("LightRadius[" + indexSTR + "]", m_pt_lights[index]->GetLightRadius());
}

void RaindropRenderer::FillFeaturesStringArray() {
	m_features_string[0] = "ftr_specular";
	m_features_string[1] = "ftr_lighting";
	m_features_string[2] = "ftr_ambient";
}

void RaindropRenderer::FillFeatureStateArray() {
	m_features_state[0] = true;
	m_features_state[1] = true;
	m_features_state[2] = true;
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
		m_CurrentShader->SetBool(m_features_string[ftr], true);
		m_features_state[ftr] = true;
	}
	else {
		std::cerr << "Feature " << m_features_string[ftr] << " already enabled." << std::endl;
	}
}

void RaindropRenderer::DisableFeature(RendererFeature ftr) {
	if (IsFeatureEnabled(ftr) == true) {
		m_CurrentShader->SetBool(m_features_string[ftr], false);
		m_features_state[ftr] = false;
	}
	else {
		std::cerr << "Feature " << m_features_string[ftr] << " already disabled." << std::endl;
	}
}

bool RaindropRenderer::IsFeatureEnabled(RendererFeature ftr) {
	return m_features_state[ftr];
}

void RaindropRenderer::SendFeaturesToShader(RD_ShaderLoader* shader) {
	shader->SetBool(m_features_string[0], m_features_state[0]);
	shader->SetBool(m_features_string[1], m_features_state[1]);
	shader->SetBool(m_features_string[2], m_features_state[2]);
}

float RaindropRenderer::GetFramerate() {
	return ((float) 1 / m_frmLmt->GetElapsedTime());
}

RD_ShaderLoader* RaindropRenderer::GetCurrentShader() {
	return m_CurrentShader;
}

double RaindropRenderer::GetLastDeltaTime() {
	return m_frmLmt->GetLastDeltaTime();
}

int RaindropRenderer::GetFrameLimit() {
	return m_frmLmt->GetFrameLimit();
}

void RaindropRenderer::RenderLightsDepth(vec3f camPos) {
	if (!IsFeatureEnabled(RendererFeature::Lighting))
		return;

	for (int i = 0; i < m_DirLights.size(); i++) {
		m_DirLights[i]->DepthRender(this, camPos);
	}
}

void RaindropRenderer::RegisterMesh(RD_Mesh* mesh) {
	std::cout << "Added mesh to renderer." << std::endl;
	m_meshes.push_back(mesh);
}

void RaindropRenderer::RenderMeshes(RD_Camera* cam) {
	for (int i = 0; i < m_meshes.size(); i++) {
		RD_ShaderLoader* shader = m_meshes[i]->GetMaterial()->GetShader();
		shader->useShader();

		if (IsFeatureEnabled(RendererFeature::Lighting)) {
			unsigned int texUnit = 0;
			int i = 0;

			for (auto dlight : m_DirLights) {
				if (!dlight->GetShadowCasting())
					continue;

				dlight->GetDepthTexID()->BindTexture(texUnit);

				shader->SetInt("ShadowMap[" + std::to_string(i) + "]", texUnit);
				shader->SetMatrix("lspaceMat[" + std::to_string(i) + "]", dlight->GetLightSpace());

				texUnit++;
				i++;
			}

			shader->SetInt("NbrDirLights", i);
		}

		m_meshes[i]->render(cam);
	}
}

void RaindropRenderer::RenderShadowMeshes() {
	if (!IsFeatureEnabled(RendererFeature::Lighting)) {
		return;
	}

	for (int i = 0; i < m_meshes.size(); i++) {
		m_meshes[i]->renderShadows(m_shadowShader);
	}
}

RD_ShaderLoader* RaindropRenderer::GetShadowShader() {
	return m_shadowShader;
}

bool RaindropRenderer::CreateGbuff() {
	int width = getWindowWidth();
    int height = getWindowHeigh();

	m_gbuffer = m_api->CreateFrameBuffer(width, height);

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

	//Shadow buff
	m_gbuffer->AddAttachement(IMGFORMAT_RGB);
	m_g_buffer.gShadows = 4;

	//Light buff
	m_gbuffer->AddAttachement(IMGFORMAT_RGB);
	m_g_buffer.gLight = 5;

	m_gbuffer->BuildFBO();

	m_light_pprocess = m_api->CreateFrameBuffer(width, height);
	//Light & PostProcess screen
	m_light_pprocess->AddAttachement(IMGFORMAT_RGB);

	m_light_pprocess->BuildFBO();

	return true;
}

bool RaindropRenderer::CreateGbuff_PBR() {
	int width = getWindowWidth();
	int height = getWindowHeigh();

	m_gbuffer = m_api->CreateFrameBuffer(width, height);

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

	//Shadow buff
	m_gbuffer->AddAttachement(IMGFORMAT_RGB);
	m_g_buffer.gShadows = 4;

	//Light buff
	m_gbuffer->AddAttachement(IMGFORMAT_RGB);
	m_g_buffer.gLight = 5;

	//Metallic/Roughness/AO
	m_gbuffer->AddAttachement(IMGFORMAT_RGB16F);
	m_g_buffer.gMetRoughAO = 6;

	m_gbuffer->BuildFBO();

	m_light_pprocess = m_api->CreateFrameBuffer(width, height);
	//Light & PostProcess screen
	m_light_pprocess->AddAttachement(IMGFORMAT_RGB);

	m_light_pprocess->BuildFBO();

	return true;
}

void RaindropRenderer::RenderGbuff(RD_Camera* cam) {
	if (!cam) {
		return;
	}

	m_gbuffer->BindFBO();
	m_api->Clear(COLOR_BUFFER | DEPTH_BUFFER);

	RenderMeshes(cam);

	m_gbuffer->UnbindFBO();

	m_light_pprocess->BindFBO();
	m_api->Clear(DEPTH_BUFFER | COLOR_BUFFER);

	RenderLightPass(cam->GetLocation());
	RenderPostProcess();

	m_light_pprocess->UnbindFBO();
}

void RaindropRenderer::RenderPostProcess() {
	for (auto pp : m_pp_effects) {
		m_api->Clear(DEPTH_BUFFER);

		pp->RenderEffect(m_light_pprocess->GetAttachementByIndex(0));
	}
}

void RaindropRenderer::RenderLightPass(vec3f CamPos) {
	SwitchShader(m_light_shader);
	SendFeaturesToShader(m_light_shader);

	m_gbuffer->GetAttachementByIndex(m_g_buffer.gAlbedo)->BindTexture(0);
	m_light_shader->SetInt("gAlbedo", 0);

	m_gbuffer->GetAttachementByIndex(m_g_buffer.gNorm)->BindTexture(1);
	m_light_shader->SetInt("gNormal", 1);

	m_gbuffer->GetAttachementByIndex(m_g_buffer.gPos)->BindTexture(2);
	m_light_shader->SetInt("gPos", 2);

	m_gbuffer->GetAttachementByIndex(m_g_buffer.gSpec)->BindTexture(3);
	m_light_shader->SetInt("gSpec", 3);

	m_gbuffer->GetAttachementByIndex(m_g_buffer.gShadows)->BindTexture(4);
	m_light_shader->SetInt("ShadowPass", 4);

	if (m_pipeline == Pipeline::PBR_ENGINE) {
		m_gbuffer->GetAttachementByIndex(m_g_buffer.gMetRoughAO)->BindTexture(5);
		m_light_shader->SetInt("gMetRoughAO", 5);
	}

	m_light_shader->SetVec3("CamPos", CamPos);

	UpdateAmbientLighting();
	UpdateDirLighting();
	UpdatePointsLighting();

	m_quad->RenderQuad();
}

void RaindropRenderer::RenderBeauty() {
	m_api->Clear(DEPTH_BUFFER | COLOR_BUFFER);

	SwitchShader(m_beauty_shader);

	m_light_pprocess->GetAttachementByIndex(0)->BindTexture(5);
	m_beauty_shader->SetInt("lightpass", 5);

	m_gui_manager->GetScreenTexture()->BindTexture(6);
	m_beauty_shader->SetInt("GUIscreen", 6);

	m_quad->RenderQuad();
}

void RaindropRenderer::UnregisterMesh(RD_Mesh* mesh) {
	int index = GetElemIndex<RD_Mesh*>(m_meshes, mesh);

	if (index != -1) {
		m_meshes.erase(m_meshes.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void RaindropRenderer::UnregisterDirLight(RD_DirLight* light) {
	int index = GetElemIndex<RD_DirLight*>(m_DirLights, light);

	if (index != -1) {
		light->Cleanup(this);
		m_DirLights.erase(m_DirLights.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void RaindropRenderer::UnregisterPointLight(RD_PointLight* ptLight) {
	int index = GetElemIndex<RD_PointLight*>(m_pt_lights, ptLight);

	if (index != -1) {
		m_pt_lights.erase(m_pt_lights.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void RaindropRenderer::UnregisterAllMeshes() {
	for (auto msh : m_meshes) {
		std::cout << "Unregistering Mesh" << std::endl;

		UnregisterMesh(msh);
		if(msh)
			delete msh;
	}

	m_meshes.clear();
}

void RaindropRenderer::UnregisterAllPointLights() {
	for (auto plight : m_pt_lights) {
		std::cout << "Unregistering Point Light" << std::endl;

		UnregisterPointLight(plight);
		if(plight)
			delete plight;
	}

	m_pt_lights.clear();
}

void RaindropRenderer::UnregisterAllDirLights() {
	for (auto dlight : m_DirLights) {
		std::cout << "Unregistering Dir Light" << std::endl;

		UnregisterDirLight(dlight);
		if(dlight)
			delete dlight;
	}

	m_DirLights.clear();
}

RD_Texture* RaindropRenderer::GetBlankTexture() {
	return m_blankTexture;
}

void RaindropRenderer::DeleteGbuff() {
	//Deleting Gbuff FBO texture & Render buffer
	
}

void RaindropRenderer::RecreateGbuff() {
	int w = m_api->GetWindowingSystem()->GetWidth();
	int h = m_api->GetWindowingSystem()->GetHeight();

	m_gbuffer->ChangeFramebufferSize(w, h);
	m_light_pprocess->ChangeFramebufferSize(w, h);
}

void RaindropRenderer::SetFullscreenMode(bool mode) {
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

RD_ShaderMaterial* RaindropRenderer::FetchShaderFromFile(std::string ref) {
	if (!std::filesystem::exists(ref)) {
		std::cerr << "Shader file " << ref << " does not exist." << std::endl;
		dispErrorMessageBox(StrToWStr("Shader file " + ref + " does not exists"));
		return nullptr;
	}

	if (m_matlib->DoMaterialExists(ref)) {
		return m_matlib->GetMaterialByName(ref);
	}

	BD_MatCustomShaderRead mread(ref);
	std::string fcode = mread.GetShaderCode();
	std::string vcode = getFileData(m_engineDir + "/Shaders/glsl/Gshad.vert");

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

bool RaindropRenderer::GetErrorFlag() {
	return m_error_flag;
}

std::string RaindropRenderer::GetEngineDir() {
	return m_engineDir;
}

void RaindropRenderer::RenderGUI_Screen() {
	m_gui_manager->RenderScreen();
}

RD_GUI_Manager* RaindropRenderer::GetGUI_Manager() {
	return m_gui_manager.get();
}

void RaindropRenderer::AddPostProcessEffect(RD_PostProcessEffect* effect) {
	std::cout << "Added new post process effect" << std::endl;
	m_pp_effects.push_back(effect);
}

void RaindropRenderer::RemovePostProcessEffect(RD_PostProcessEffect* effect) {
	int index = GetElemIndex<RD_PostProcessEffect*>(m_pp_effects, effect);

	if (index != -1) {
		std::cout << "Removed post process effect." << std::endl;

		m_pp_effects.erase(m_pp_effects.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}
