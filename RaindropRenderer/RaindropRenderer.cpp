#include "pch.h"
#include "RaindropRenderer.h"

RaindropRenderer::RaindropRenderer(int w, int h, std::string windowName) : m_height(h), m_width(w) {
	initWindow(w, h, windowName);
	
	RD_ShaderLoader* shader = new RD_ShaderLoader();
	shader->compileShaderFromFile("vshader.vert", "fshader.frag");
	m_shader = shader;
	m_shader->useShader();

	ambientStrength = 1.0f;
	ambientColor = vec3f(1.0f, 1.0f, 1.0f);
	UpdateLighting();
}

RaindropRenderer::~RaindropRenderer() {
	glfwDestroyWindow(win);
	//delete m_shader;
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
	glClearColor(refreshColor.getX(), refreshColor.getY(), refreshColor.getZ(), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RaindropRenderer::SwapWindow() {
	glfwSwapBuffers(win);
	glfwPollEvents();
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

void RaindropRenderer::SetAmbientStrength(float strength) {
	ambientStrength = strength;

	UpdateLighting();
}

void RaindropRenderer::SetAmbientColor(vec3f nColor) {
	ambientColor = nColor;

	UpdateLighting();
}

void RaindropRenderer::UpdateLighting() {
	m_shader->SetFloat("AmbientStrength", ambientStrength);
	m_shader->SetVec3("AmbientColor", ambientColor);
}