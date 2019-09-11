#include "pch.h"
#include "RaindropRenderer.h"

RaindropRenderer::RaindropRenderer(int w, int h, std::string windowName) : m_height(h), m_width(w) {
	initWindow(w, h, windowName);

	RD_FrameLimiter* flmt = new RD_FrameLimiter(60);
	m_frmLmt = flmt;
	
	RD_ShaderLoader* shader = new RD_ShaderLoader();
	shader->compileShaderFromFile("Engine/Shaders/GameView.vert", "Engine/Shaders/GameView.frag");
	m_shader = shader;
	m_shader->useShader();

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
	glEnable(GL_DEPTH_TEST);
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
	return m_height;
}

int RaindropRenderer::getWindowWidth() {
	return m_width;
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
		m_DBG_light_mdl->render();
	}
}

void RaindropRenderer::UpdatePointsLighting() {
	m_shader->SetVec3("LightPos", m_pt_lights[0]->GetPosition());
	m_shader->SetVec3("LightBrightness", m_pt_lights[0]->GetBrightness());
}

GLFWwindow* RaindropRenderer::GetGLFWwindow() {
	return win;
}