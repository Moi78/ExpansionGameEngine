#include "pch.h"
#include "RaindropRenderer.h"

#include "RD_DirLight.h"
#include "RD_Mesh.h"
#include "RD_Quad.h"
//#include "RD_FrameBuffer.h"
#include "RD_Camera.h"
#include "RD_MaterialLibrary.h"
#include "RD_GUI_Manager.h"
#include "RD_PostProcess.h"

#include "RD_RenderingAPI.h"
#include "RD_RenderingAPI_GL.h"

RaindropRenderer::RaindropRenderer(int w, int h, std::string windowName, API api, int maxFramerate, bool minInit, std::string engineDir) {
	FillFeaturesStringArray();
	FillFeatureStateArray();

	m_engineDir = engineDir;

	if (api == API::OPENGL) {
		m_api = std::make_unique<RD_RenderingAPI_GL>(this);
	}

	m_error_flag = false;

	m_api->InitializeAPI(w, h, windowName);

	m_frmLmt = std::make_unique<RD_FrameLimiter>(maxFramerate);

	//Shader Compiling
	std::cout << "Compiling main shaders..." << std::endl;

	m_shadowShader = std::make_unique<RD_ShaderLoader>();
	m_shadowShader->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Shadow.vert", m_engineDir + "/Shaders/glsl/Shadow.frag");

	m_light_shader = std::make_unique<RD_ShaderLoader>();
	m_light_shader->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Light.vert", m_engineDir + "/Shaders/glsl/Light.frag");

	m_beauty_shader = std::make_unique<RD_ShaderLoader>();
	m_beauty_shader->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Beauty.vert", m_engineDir + "/Shaders/glsl/Beauty.frag");

	m_CurrentShader = m_light_shader.get();

	m_defTex = m_api->CreateTexture();
	m_defTex->LoadTexture(m_engineDir + "/Textures/defTex.png");

	m_blankTexture = m_api->CreateTexture();
	m_blankTexture->GenerateColorTex(vec3f(1.0f, 1.0f, 1.0f));

	if (RENDER_DEBUG_ENABLED) {
		RD_ShaderLoader* shad = new RD_ShaderLoader();
		shad->compileShaderFromFile(m_engineDir + "/Shaders/glsl/Debug.vert", m_engineDir + "/Shaders/glsl/Debug.frag");
		RD_ShaderMaterial* dbgmat = new RD_ShaderMaterial(shad);

		m_DBG_light_mdl = std::make_unique<RD_Mesh>(this, dbgmat, vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.3f, 0.3f, 0.3f));
		m_DBG_light_mdl->loadMesh(m_engineDir + "/Meshes/Light.msh");
	}

	ambientStrength = 1.0f;
	ambientColor = vec3f(1.0f, 1.0f, 1.0f);

	CreateGbuff();

	m_gui_manager = std::make_unique<RD_GUI_Manager>(this);
	m_gui_manager->InitManager();

	m_quad = std::make_unique<RD_Quad>();
	m_quad->Bufferize();

	m_matlib = std::make_unique<RD_MaterialLibrary>();
}

RaindropRenderer::~RaindropRenderer() {
	m_pt_lights.clear();
	m_DirLights.clear();
	m_meshes.clear();

	delete m_gbuffer;
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

	//UpdateDirLighting();

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

		if(IsFeatureEnabled(RendererFeature::Lighting))
			DisableFeature(RendererFeature::Lighting);
		else
			rEnableLighting = false;

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gbuffer->GetFBO());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, getWindowWidth(), getWindowHeigh(), 0, 0, getWindowWidth(), getWindowHeigh(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		for (int i = 0; i < m_pt_lights.size(); i++) {
			m_DBG_light_mdl->GetMaterial()->GetShader()->useShader();

			m_DBG_light_mdl->SetPosition(m_pt_lights[i]->GetPosition());
			m_DBG_light_mdl->render(cam, RenderMode::Wireframe);
		}

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

//GLFWwindow* RaindropRenderer::GetGLFWwindow() {
//	return win;
//}

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

void RaindropRenderer::SetAASampling(int nbs) {
	glfwWindowHint(GLFW_SAMPLES, nbs);
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
			unsigned int texUnit = GL_TEXTURE0;
			int i = 0;

			for (auto dlight : m_DirLights) {
				if (!dlight->GetShadowCasting())
					continue;

				glActiveTexture(texUnit);
				glBindTexture(GL_TEXTURE_2D, dlight->GetDepthTexID());

				shader->SetInt("ShadowMap[" + std::to_string(i) + "]", texUnit - 0x84C0);
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
		m_meshes[i]->renderShadows(m_shadowShader.get());
	}
}

RD_ShaderLoader* RaindropRenderer::GetShadowShader() {
	return m_shadowShader.get();
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

	return true;
}

void RaindropRenderer::RenderGbuff(RD_Camera* cam) {
	if (!cam) {
		return;
	}

	m_gbuffer->BindFBO();
	m_api->Clear(COLOR_BUFFER | DEPTH_BUFFER);

	glCullFace(GL_BACK);

	RenderMeshes(cam);

	RenderLightPass(cam->GetLocation());
	RenderPostProcess();

	glCullFace(GL_FRONT);
	m_gbuffer->UnbindFBO();
}

void RaindropRenderer::RenderPostProcess() {
	for (auto pp : m_pp_effects) {
		m_api->Clear(DEPTH_BUFFER);

		pp->RenderEffect(m_g_buffer.gLight);
	}
}

void RaindropRenderer::RenderLightPass(vec3f CamPos) {
	m_api->Clear(DEPTH_BUFFER);

	SwitchShader(m_light_shader.get());
	SendFeaturesToShader(m_light_shader.get());

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_g_buffer.gAlbedo);
	m_gbuffer->GetAttachementByIndex(m_g_buffer.gAlbedo)->BindTexture(0);
	m_light_shader->SetInt("gAlbedo", 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, m_g_buffer.gNorm);
	m_gbuffer->GetAttachementByIndex(m_g_buffer.gNorm)->BindTexture(1);
	m_light_shader->SetInt("gNormal", 1);

	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, m_g_buffer.gPos);
	m_gbuffer->GetAttachementByIndex(m_g_buffer.gPos)->BindTexture(2);
	m_light_shader->SetInt("gPos", 2);

	//glActiveTexture(GL_TEXTURE3);
	//glBindTexture(GL_TEXTURE_2D, m_g_buffer.gSpec);
	m_gbuffer->GetAttachementByIndex(m_g_buffer.gSpec)->BindTexture(3);
	m_light_shader->SetInt("gSpec", 3);

	//glActiveTexture(GL_TEXTURE4);
	//glBindTexture(GL_TEXTURE_2D, m_g_buffer.gShadows);
	m_gbuffer->GetAttachementByIndex(m_g_buffer.gShadows)->BindTexture(4);
	m_light_shader->SetInt("ShadowPass", 4);

	m_light_shader->SetVec3("CamPos", CamPos);

	UpdateAmbientLighting();
	UpdateDirLighting();
	UpdatePointsLighting();

	m_quad->RenderQuad();
}

void RaindropRenderer::RenderBeauty() {
	m_api->Clear(DEPTH_BUFFER | COLOR_BUFFER);

	SwitchShader(m_beauty_shader.get());

	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, m_g_buffer.gLight);
	m_gbuffer->GetAttachementByIndex(m_g_buffer.gLight)->BindTexture(5);
	m_beauty_shader->SetInt("lightpass", 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_gui_manager->GetScreenTexture());
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
		delete msh;
	}

	m_meshes.clear();
}

void RaindropRenderer::UnregisterAllPointLights() {
	for (auto plight : m_pt_lights) {
		std::cout << "Unregistering Point Light" << std::endl;

		UnregisterPointLight(plight);
		delete plight;
	}

	m_pt_lights.clear();
}

void RaindropRenderer::UnregisterAllDirLights() {
	for (auto dlight : m_DirLights) {
		std::cout << "Unregistering Dir Light" << std::endl;

		UnregisterDirLight(dlight);
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
}

void RaindropRenderer::SetFullscreenMode(bool mode) {
	m_api->GetWindowingSystem()->SetFullscreenMode(mode);
}

void RaindropRenderer::AddToTextureGarbageCollector(unsigned int texID) {
	m_textureGarbageCollector.push_back(texID);
}

void RaindropRenderer::EmptyTextureGarbageCollector() {
	for (auto tex : m_textureGarbageCollector) {
		glDeleteTextures(1, &tex);
	}
}

void RaindropRenderer::AddToFramebufferGarbageCollector(unsigned int fboID) {
	m_framebufferGarbageCollector.push_back(fboID);
}

void RaindropRenderer::EmptyFramebufferGarbageCollector() {
	for (auto fbo : m_framebufferGarbageCollector) {
		glDeleteFramebuffers(1, &fbo);
	}
}

RD_ShaderMaterial* RaindropRenderer::FetchShaderFromFile(std::string ref) {
	if (!std::filesystem::exists(ref)) {
		std::cerr << "Shader file " << ref << " does not exist." << std::endl;
		return nullptr;
	}

	if (m_matlib->DoMaterialExists(ref)) {
		return m_matlib->GetMaterialByName(ref);
	}

	BD_MatCustomShaderRead mread(ref);
	std::string fcode = mread.GetShaderCode();
	std::string vcode = getFileData(m_engineDir + "/Shaders/glsl/Gshad.vert");

	RD_ShaderLoader* shader = new RD_ShaderLoader();
	shader->CompileShaderFromCode(vcode, fcode);

	RD_ShaderMaterial* shdmat = new RD_ShaderMaterial(shader);
	for (int i = 0; i < mread.GetTextureCount(); i++) {
		RD_Texture* tex = m_api->CreateTexture();
		tex->LoadTexture(mread.GetTexturePath(i));

		shdmat->AddTexture(mread.GetTextureParamName(i), tex->GetTextureID());
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

//----------------------------------------------------------------------------------------------------------------------------------------------------

void glfwWinCallback_(GLFWwindow* win, int w, int h) {
	RaindropRenderer* rndr = (RaindropRenderer*)glfwGetWindowUserPointer(win);
	rndr->RecreateGbuff();
	
	glViewport(0, 0, w, h);

	rndr->GetGUI_Manager()->RebuildFramebuffer();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------

std::wstring StrToWStr(std::string str) {
	std::wstring wstr(str.begin(), str.end());
	return wstr;
}
