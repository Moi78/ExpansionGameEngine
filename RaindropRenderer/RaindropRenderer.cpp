#include "pch.h"
#include "RaindropRenderer.h"

RaindropRenderer::RaindropRenderer(int w, int h, std::string windowName, bool minInit) : m_height(h), m_width(w) {
	FillFeaturesStringArray();
	FillFeatureStateArray();

	if (!minInit) {
		initWindow(w, h, windowName);
	}
	else {
		this->MinInit();
	}

	m_frmLmt = new RD_FrameLimiter(60);
	
	m_shader = new RD_ShaderLoader();
	m_shader->compileShaderFromFile("Engine/Shaders/GameView.vert", "Engine/Shaders/GameView.frag");
	m_shader->useShader();

	EnableAllFeatures();

	m_defTex = new RD_Texture();
	m_defTex->LoadTexture("Engine/Textures/defTex.png");

	m_mdef = {};
	m_mdef.BaseColor = m_defTex->GetTextureID();
	m_mdef.SpecularColor = vec3f(1.0f, 1.0f, 1.0f);
	m_mdef.SpecularExp = 2.0f;

	if (RENDER_DEBUG_ENABLED) {
		m_DBG_light_mdl = new RD_Mesh(m_shader, m_mdef, vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.1f, 0.1f, 0.1f));
		m_DBG_light_mdl->loadMesh("Engine/Meshes/Light.msh");

		m_DBG_sound_emitter_mdl = new RD_Mesh(m_shader, m_mdef, vec3f(), vec3f(), vec3f(0.2f, 0.2f, 0.2f));
		m_DBG_sound_emitter_mdl->loadMesh("Engine/Meshes/snd_emitter.msh");
	}

	ambientStrength = 1.0f;
	ambientColor = vec3f(1.0f, 1.0f, 1.0f);

	UpdateAmbientLighting();

	m_gview_shader_in_use = true;
	m_CurrentShader = m_shader;

	InitGUI();
}

RaindropRenderer::~RaindropRenderer() {
	glfwDestroyWindow(win);
}

void RaindropRenderer::initWindow(int w, int h, std::string name) {
	if (!glfwInit()) {
		dispErrorMessageBox(TEXT("Cannot init GLFW"));
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	win = glfwCreateWindow(w, h, name.c_str(), NULL, NULL);

	if (win == NULL) {
		dispErrorMessageBox(TEXT("Cannot create the window."));
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(win);

	initGlad();

	glViewport(0, 0, m_width, m_height);

	glfwSetFramebufferSizeCallback(win, glfwWinCallback);

	glEnable(GL_DEPTH_TEST);
}

void RaindropRenderer::MinInit() {
	initGlad(true);

	glViewport(0, 0, m_width, m_height);

	glEnable(GL_DEPTH_TEST);
}

void glfwWinCallback(GLFWwindow* win, int w, int h) {
	glViewport(0, 0, w, h);
}

void RaindropRenderer::ClearWindow(vec3f refreshColor) {
	glfwPollEvents();

	m_frmLmt->start();
	glClearColor(refreshColor.getX(), refreshColor.getY(), refreshColor.getZ(), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RaindropRenderer::SwapWindow() {
	glfwSwapBuffers(win);

	m_frmLmt->stop();

	if (m_frmLmt->GetElapsedTime() < (float)1 / m_frmLmt->GetFrameLimit()) {

		while (m_frmLmt->GetElapsedTime() < (float)1 / m_frmLmt->GetFrameLimit()) {
			m_frmLmt->stop();
		}
	}
}

bool RaindropRenderer::WantToClose() {
	if (glfwWindowShouldClose(win)) {
		return true;
	}
	else {
		return false;
	}
}

void RaindropRenderer::initGlad(bool minInit) {
	if (!minInit) {
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			dispErrorMessageBox(TEXT("Cannot init GLAD."));
			exit(-1);
		}
	}
	else {
		if (gladLoadGL() != 1) {
			dispErrorMessageBox(TEXT("Cannot init GLAD"));
			exit(-1);
		}
	}
}

int RaindropRenderer::getWindowHeigh() {
	int h;
	glfwGetWindowSize(win, NULL, &h);
	return h;
}

int RaindropRenderer::getWindowWidth() {
	int w;
	glfwGetWindowSize(win, &w, NULL);
	return w;
}

RD_ShaderLoader* RaindropRenderer::GetShader() {
	return m_shader;
}

RD_ShaderLoader* RaindropRenderer::GetLightShader() {
	return m_LightShader;
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
	m_shader->SetFloat("AmbientStrength", ambientStrength);
	m_shader->SetVec3("AmbientColor", ambientColor);
}

int RaindropRenderer::AppendLight(RD_PointLight* ptLight) {
	std::cout << "Registering light color : " << ptLight->GetColor().getX() << " " << ptLight->GetColor().getY() << " " << ptLight->GetColor().getZ() << " brightness : " << ptLight->GetBrightness() << std::endl;

	m_pt_lights.push_back(ptLight);

	int lightIndex = std::size(m_pt_lights);

	UpdatePointsLighting();

	return lightIndex;
}

int RaindropRenderer::AppendDirLight(RD_DirLight* dirLight) {
	m_DirLights.push_back(dirLight);

	UpdateDirLighting();

	return 1;
}

void RaindropRenderer::UpdateDirLighting() {
	for (int i = 0; i < m_DirLights.size(); i++) {
		FillDirLightIndice(i);
	}

	m_shader->SetInt("nbrDirLight", m_DirLights.size());
}

void RaindropRenderer::FillDirLightIndice(int index) {
	if (index > m_DirLights.size()) {
		std::cerr << "Can't add this directionnal light : Index out of range." << std::endl;
		return;
	}
	else if (index > 10) {
		std::cerr << "No more than 10 Directional Lights are supported." << std::endl;
		return;
	}

	std::string indexSTR = std::to_string(index);

	m_shader->SetVec3("DirLightDir[" + indexSTR + "]", m_DirLights[index]->GetLightDir());
	m_shader->SetVec3("DirLightColor[" + indexSTR + "]", m_DirLights[index]->GetLightColor());
	m_shader->SetFloat("DirLightBrightness[" + indexSTR + "]", m_DirLights[index]->GetBrightness());
}

void RaindropRenderer::SwitchShader(RD_ShaderLoader* shader) {
	m_CurrentShader = shader;
	m_CurrentShader->useShader();
}

void RaindropRenderer::RenderDbg() {
	if (RENDER_DEBUG_ENABLED) {
		DisableFeature(RendererFeature::Lighting);

		for (int i = 0; i < m_pt_lights.size(); i++) {
			//mdef.Color = m_pt_lights[i]->GetColor();

			m_DBG_light_mdl->SetPosition(m_pt_lights[i]->GetPosition());
			//m_DBG_light_mdl->UpdateMaterial(&mdef);
			m_DBG_light_mdl->render(RenderMode::Wireframe);
		}

		for (int i = 0; i < m_sound_emitters.size(); i++) {
			m_DBG_sound_emitter_mdl->SetPosition(m_sound_emitters[i]->getLocation());
			m_DBG_sound_emitter_mdl->render(RenderMode::Wireframe);
		}

		EnableFeature(RendererFeature::Lighting);
	}
}

void RaindropRenderer::UpdatePointsLighting() {
	for (int i = 0; i < m_pt_lights.size(); i++) {
		FillPtLightIndice(i);
	}

	m_shader->SetInt("nbrPointLight", m_pt_lights.size());
}

void RaindropRenderer::FillPtLightIndice(int index) {
	if (m_pt_lights.size() < index) {
		std::cerr << "Can't add point light : Index out of range." << std::endl;
	}
	else if (index > 500) {
		std::cerr << "Can't add point light : No more than 500 lights are supported." << std::endl;
	}

	std::string indexSTR = std::to_string(index);

	m_shader->SetVec3("LightPos["+ indexSTR +"]", m_pt_lights[index]->GetPosition());
	m_shader->SetFloat("LightBrightness[" + indexSTR + "]", m_pt_lights[index]->GetBrightness());
	m_shader->SetVec3("LightColor[" + indexSTR + "]", m_pt_lights[index]->GetColor());
}

GLFWwindow* RaindropRenderer::GetGLFWwindow() {
	return win;
}

void RaindropRenderer::FillFeaturesStringArray() {
	m_features_string[0] = "ftr_specular";
	m_features_string[1] = "ftr_lighting";
	m_features_string[2] = "ftr_texture";
}

void RaindropRenderer::FillFeatureStateArray() {
	m_features_state[0] = false;
	m_features_state[1] = false;
	m_features_state[2] = false;
}

void RaindropRenderer::EnableAllFeatures() {
	for (int ftr = RendererFeature::Lighting; ftr != RendererFeature::Textures; ftr += 1) {
		RendererFeature enb_ftr = static_cast<RendererFeature>(ftr);
		EnableFeature(enb_ftr);
	}
}

//Disable or Enable features
void RaindropRenderer::EnableFeature(RendererFeature ftr) {
	if (!IsFeatureEnabled(ftr)) {
		m_shader->SetBool(m_features_string[ftr], true);
		m_features_state[ftr] = true;
	}
	else {
		std::cerr << "Feature " << m_features_string[ftr] << " already enabled." << std::endl;
	}
}

void RaindropRenderer::DisableFeature(RendererFeature ftr) {
	if (IsFeatureEnabled(ftr)) {
		m_shader->SetBool(m_features_string[ftr], false);
		m_features_state[ftr] = false;
	}
	else {
		std::cerr << "Feature " << m_features_string[ftr] << " already disabled." << std::endl;
	}
}

bool RaindropRenderer::IsFeatureEnabled(RendererFeature ftr) {
	return m_features_state[ftr];
}

float RaindropRenderer::GetFramerate() {
	return 1 / m_frmLmt->GetElapsedTime();;
}

RD_ShaderLoader* RaindropRenderer::DBG_GetGameViewShader() {
	return m_shader;
}

RD_ShaderLoader* RaindropRenderer::DBG_GetLightShader() {
	return m_LightShader;
}

RD_ShaderLoader* RaindropRenderer::GetCurrentShader() {
	return m_CurrentShader;
}

void RaindropRenderer::RegisterSoundEmitter(PS_Emitter* emitter) {
	m_sound_emitters.push_back(emitter);
}

void RaindropRenderer::InitGUI() {

}

void RaindropRenderer::RegisterGUI(RD_GUI* gui) {
	m_guis.push_back(gui);
}

double RaindropRenderer::GetLastDeltaTime() {
	return m_frmLmt->GetLastDeltaTime();
}

int RaindropRenderer::GetFrameLimit() {
	return m_frmLmt->GetFrameLimit();
}