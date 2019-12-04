#include "pch.h"
#include "RaindropRenderer.h"

RaindropRenderer::RaindropRenderer(int w, int h, std::string windowName) : m_height(h), m_width(w) {
	FillFeaturesStringArray();
	FillFeatureStateArray();

	initWindow(w, h, windowName);

	RD_FrameLimiter* flmt = new RD_FrameLimiter(60);
	m_frmLmt = flmt;
	
	RD_ShaderLoader* shader = new RD_ShaderLoader();
	shader->compileShaderFromFile("Engine/Shaders/GameView.vert", "Engine/Shaders/GameView.frag");
	m_shader = shader;
	m_shader->useShader();

	EnableAllFeatures();

	RD_ShaderLoader* lshader = new RD_ShaderLoader();
	lshader->compileShaderFromFile("Engine/Shaders/Lights.vert", "Engine/Shaders/Lights.frag");
	m_LightShader = lshader;

	BD_MatDef mdef = {};
	mdef.Color = vec3f(1.0f, 1.0f, 1.0f);

	RD_Mesh* lmsh = new RD_Mesh(m_shader, mdef, vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.3f, 0.3f, 0.3f));
	lmsh->loadMesh("Engine/Meshes/Light.msh");

	m_DBG_light_mdl = lmsh;

	ambientStrength = 1.0f;
	ambientColor = vec3f(1.0f, 1.0f, 1.0f);

	UpdateAmbientLighting();
}

RaindropRenderer::~RaindropRenderer() {
	glfwDestroyWindow(win);
}

void RaindropRenderer::initWindow(int w, int h, std::string name) {
	if (!glfwInit()) {
		dispErrorMessageBox(TEXT("Cannot init GLFW"));
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

void glfwWinCallback(GLFWwindow* win, int w, int h) {
	glViewport(0, 0, w, h);
}

void RaindropRenderer::ClearWindow(vec3f refreshColor) {
	m_frmLmt->start();
	glClearColor(refreshColor.getX(), refreshColor.getY(), refreshColor.getZ(), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RaindropRenderer::SwapWindow() {
	glfwSwapBuffers(win);
	glfwPollEvents();

	m_frmLmt->stop();

	if (m_frmLmt->GetElapsedTime() < (float)1 / m_frmLmt->GetFrameLimit()) {

		while (m_frmLmt->GetElapsedTime() < (float)1 / m_frmLmt->GetFrameLimit()) {
			m_frmLmt->stop();
		}
	}

	double fps = 1 / m_frmLmt->GetElapsedTime();
	//std::cout << "FPS : " << fps << std::endl;
}

bool RaindropRenderer::WantToClose() {
	if (glfwWindowShouldClose(win)) {
		return true;
	}
	else {
		return false;
	}
}

void RaindropRenderer::initGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		dispErrorMessageBox(TEXT("Cannot init GLAD."));
		exit(-1);
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
	m_pt_lights.push_back(ptLight);

	int lightIndex = std::size(m_pt_lights);

	UpdatePointsLighting();

	return lightIndex;
}

void RaindropRenderer::SwitchShader(RD_ShaderLoader* shader) {
	shader->useShader();
}

void RaindropRenderer::RenderDbg() {
	//SwitchShader(m_LightShader);
	for (int i = 0; i < m_pt_lights.size(); i++) {
		m_DBG_light_mdl->SetPosition(m_pt_lights[i]->GetPosition());
		m_DBG_light_mdl->render(RenderMode::Wireframe);
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