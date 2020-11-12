#include "pch.h"
#include "RD_RenderingAPI_GL.h"

#include "RD_GUI_Manager.h"

//RD_WindowingSystemGLFW
RD_WindowingSystemGLFW::RD_WindowingSystemGLFW(RaindropRenderer* rndr) : RD_WindowingSystem() {
	m_rndr = rndr;
}

RD_WindowingSystemGLFW::~RD_WindowingSystemGLFW() {
	glfwDestroyWindow(m_win);
}

bool RD_WindowingSystemGLFW::OpenWindow(std::string name, int w, int h) {
	m_w = w;
	m_h = h;

	if (!glfwInit()) {
		const char* error;
		glfwGetError(&error);

		dispErrorMessageBox(StrToWStr("Cannot initialize GLFW. " + std::string(error)));
		glfwTerminate();
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_win = glfwCreateWindow(w, h, name.c_str(), NULL, NULL);
	if (m_win == NULL) {
		const char* error;
		glfwGetError(&error);

		dispErrorMessageBox(StrToWStr("Cannot initialize GLFW. " + std::string(error)));
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_win);

	glfwSetWindowUserPointer(m_win, m_rndr);
	glfwSetFramebufferSizeCallback(m_win, glfwWinCallback);
}

void RD_WindowingSystemGLFW::SetFullscreenMode(bool mode) {
	if (mode) {
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor) {
			std::cerr << "Cannot turn on fullscreen mode" << std::endl;
		}

		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwSetWindowMonitor(m_win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else {
		glfwSetWindowMonitor(m_win, nullptr, 0, 0, m_w, m_h, 60);
	}
}

int RD_WindowingSystemGLFW::GetHeight() {
	return m_h;
}

int RD_WindowingSystemGLFW::GetWidth() {
	return m_w;
}

GLFWwindow* RD_WindowingSystemGLFW::GetWindow() {
	return m_win;
}

void RD_WindowingSystemGLFW::SwapWindow() {
	glfwSwapBuffers(m_win);
}

bool RD_WindowingSystemGLFW::WantToClose() {
	return glfwWindowShouldClose(m_win);
}

bool RD_WindowingSystemGLFW::GetKeyPress(int key) {
	return glfwGetKey(m_win, key) == GLFW_PRESS;
}

bool RD_WindowingSystemGLFW::GetMouseButton(int button) {
	return glfwGetMouseButton(m_win, button) == GLFW_PRESS;
}

void RD_WindowingSystemGLFW::CaptureCursor(bool mode) {
	if (mode) {
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

double RD_WindowingSystemGLFW::GetCursorPosX() {
	double xaxis;
	glfwGetCursorPos(m_win, &xaxis, NULL);

	return xaxis;
}

double RD_WindowingSystemGLFW::GetCursorPosY() {
	double yaxis;
	glfwGetCursorPos(m_win, NULL, &yaxis);

	return yaxis;
}

void RD_WindowingSystemGLFW::SetCursorPos(double x, double y) {
	glfwSetCursorPos(m_win, x, y);
}

void RD_WindowingSystemGLFW::PollEvents() {
	glfwPollEvents();
}

void glfwWinCallback(GLFWwindow* win, int w, int h) {
	RaindropRenderer* rndr = (RaindropRenderer*)glfwGetWindowUserPointer(win);
	rndr->RecreateGbuff();

	glViewport(0, 0, w, h);

	rndr->GetGUI_Manager()->RebuildFramebuffer();
}

//RD_Rendering_API_GL
RD_RenderingAPI_GL::RD_RenderingAPI_GL(RaindropRenderer* rndr) : RD_RenderingAPI() {
	m_win_sys = new RD_WindowingSystemGLFW(rndr);
}

RD_RenderingAPI_GL::~RD_RenderingAPI_GL() {

}

bool RD_RenderingAPI_GL::InitializeAPI(int w, int h, std::string wname) {
	if (!m_win_sys->OpenWindow(wname, w, h)) {
		dispErrorMessageBox(StrToWStr("Cannot initialize GLFW."));
		return false;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		dispErrorMessageBox(StrToWStr("Cannot init GLAD."));
		return false;
	}

	glViewport(0, 0, w, h);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	return true;
}

RD_RenderingAPI_VertexElemBufferGL* RD_RenderingAPI_GL::CreateVertexElemBuffer() {
	return new RD_RenderingAPI_VertexElemBufferGL();
}

RD_Texture* RD_RenderingAPI_GL::CreateTexture() {
	return new RD_Texture_GL();
}


RD_FrameBuffer* RD_RenderingAPI_GL::CreateFrameBuffer(int w, int h) {
	return new RD_FrameBuffer_GL(w, h);
}

void RD_RenderingAPI_GL::Draw(RD_RenderingAPI_VertexElemBuffer* vbuff, DrawMode rndrMode) {
	vbuff->BindBuffer();
	glDrawElements(GL_TRIANGLES, 0, GL_UNSIGNED_INT, 0);
	vbuff->UnbindBuffer();
}

void RD_RenderingAPI_GL::SetFilledMode(FillingMode fmode) {

}

RD_WindowingSystem* RD_RenderingAPI_GL::GetWindowingSystem() {
	return m_win_sys;
}

void RD_RenderingAPI_GL::Clear(int mask) {
	if (mask & DEPTH_BUFFER)
		glClear(GL_DEPTH_BUFFER_BIT);

	if (mask & COLOR_BUFFER)
		glClear(GL_COLOR_BUFFER_BIT);
}

//RD_RenderingAPI_VertexElemBufferGL

RD_RenderingAPI_VertexElemBufferGL::RD_RenderingAPI_VertexElemBufferGL() : RD_RenderingAPI_VertexElemBuffer() {
	VAO = 0;
	VBO = 0;
	EBO = 0;

	elem_count = 0;
}

RD_RenderingAPI_VertexElemBufferGL::~RD_RenderingAPI_VertexElemBufferGL() {
	DeleteBuffer();
}

void RD_RenderingAPI_VertexElemBufferGL::CreateBuffer() {
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void RD_RenderingAPI_VertexElemBufferGL::FillBufferData(float* data, int count, unsigned int* elemData, int elemCount) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elemCount * sizeof(unsigned int), elemData, GL_STATIC_DRAW);

	size_t elemSize = 8;

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, elemSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, elemSize * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//UV Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, elemSize * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	elem_count = elemCount;
}

void RD_RenderingAPI_VertexElemBufferGL::BindBuffer() {
	glBindVertexArray(VAO);
}

void RD_RenderingAPI_VertexElemBufferGL::UnbindBuffer() {
	glBindVertexArray(0);
}

void RD_RenderingAPI_VertexElemBufferGL::DeleteBuffer() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

unsigned int RD_RenderingAPI_VertexElemBufferGL::GetElementCount() {
	return elem_count;
}