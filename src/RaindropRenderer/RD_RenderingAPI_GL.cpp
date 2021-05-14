#include "pch.h"
#include "RD_RenderingAPI_GL.h"

#include "RD_GUI_Manager.h"

//---------------------------------------------  RD_WindowingSystemGLFW  ---------------------------------------------

#ifdef BUILD_OPENGL

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	m_win = glfwCreateWindow(w, h, name.c_str(), NULL, NULL);
	if (m_win == NULL) {
		const char* error;
		glfwGetError(&error);

		std::cerr << "GLFW ERROR : " << error << std::endl;

		dispErrorMessageBox(StrToWStr("Cannot initialize GLFW. " + std::string(error)));
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_win);

	glfwSetWindowUserPointer(m_win, m_rndr);
	glfwSetFramebufferSizeCallback(m_win, glfwWinCallback);

	return true;
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
	glfwGetWindowSize(m_win, NULL, &m_h);
	return m_h;
}

int RD_WindowingSystemGLFW::GetWidth() {
	glfwGetWindowSize(m_win, &m_w, NULL);
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

void RD_WindowingSystemGLFW::SetVSync(const bool vsync) {
	if (vsync) {
		glfwSwapInterval(1);
	} else {
		glfwSwapInterval(0);
	}
}

void glfwWinCallback(GLFWwindow* win, int w, int h) {
	RaindropRenderer* rndr = (RaindropRenderer*)glfwGetWindowUserPointer(win);
	rndr->ResizeViewport(vec2f(0.0f, 0.0f), vec2f(w, h));
	rndr->GetGUI_Manager()->RebuildFramebuffer();
}

//---------------------------------------------  RD_Rendering_API_GL ---------------------------------------------

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
	glEnable(GL_CULL_FACE);

	if (CheckExtensionAvailability("GL_ARB_bindless_texture")) {
		glEnable(GL_ARB_bindless_texture);
		std::cout << "GL_ARB_bindless_texture supported !" << std::endl;
	}
	else {
		std::cerr << "ERROR: GL_ARB_bindless_texture is not supported." << std::endl;
	}

	return true;
}

RD_RenderingAPI_VertexElemBufferGL* RD_RenderingAPI_GL::CreateVertexElemBuffer() {
	return new RD_RenderingAPI_VertexElemBufferGL();
}

RD_Texture* RD_RenderingAPI_GL::CreateTexture() {
	return new RD_Texture_GL();
}


RD_FrameBuffer* RD_RenderingAPI_GL::CreateFrameBuffer(int w, int h, bool nodepth) {
	return new RD_FrameBuffer_GL(w, h, nodepth);
}

void RD_RenderingAPI_GL::Draw(RD_RenderingAPI_VertexElemBuffer* vbuff) {
	glDrawElements(GL_TRIANGLES, vbuff->GetElementCount(), GL_UNSIGNED_INT, 0);
}

void RD_RenderingAPI_GL::SetFilledMode(FillingMode fmode) {
	if (fmode == FillingMode::FILLED) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
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

void RD_RenderingAPI_GL::SetClearColor(const vec3f& color) {
	glClearColor(color.getX(), color.getY(), color.getZ(), 1.0f);
}


RD_ShaderLoader* RD_RenderingAPI_GL::CreateShader() {
	return new RD_ShaderLoader_GL();
}

RD_Cubemap* RD_RenderingAPI_GL::CreateCubemap() {
	return new RD_Cubemap_GL();
}

RD_UniformBuffer* RD_RenderingAPI_GL::CreateUniformBuffer(const size_t bufferSize, const int binding) {
	return new RD_UniformBuffer_GL(bufferSize, binding);
}

RD_ShaderStorageBuffer* RD_RenderingAPI_GL::CreateShaderStorageBuffer(const size_t bufferSize, const int binding) {
	return new RD_ShaderStorageBuffer_GL(bufferSize, binding);
}

void RD_RenderingAPI_GL::SetViewportSize(int w, int h, int x, int y) {
	glViewport(x, y, w, h);
}

RD_RenderingAPI_VertexBuffer* RD_RenderingAPI_GL::CreateVertexBuffer() {
	return new RD_RenderingAPI_VertexBufferGL();
}

void RD_RenderingAPI_GL::DrawVB(RD_RenderingAPI_VertexBuffer* vbuff, DrawMode dm) {
	glDrawArrays(dm + 4, 0, vbuff->GetFloatCount() / 8);
}

int RD_RenderingAPI_GL::GetMaxTextureCount() {
	int rdata = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &rdata);

	return rdata;
}

void RD_RenderingAPI_GL::DrawInstanced(RD_RenderingAPI_VertexBufferInstanced* vbuff, const int nbr, DrawMode dm) {
	glDrawArraysInstanced(dm + 4, 0, vbuff->GetFloatCount() / 8, nbr);
}

RD_RenderingAPI_VertexBufferInstanced* RD_RenderingAPI_GL::CreateVertexBufferInstanced() {
	return new RD_RenderingAPI_VertexBufferInstancedGL();
}

void RD_RenderingAPI_GL::EnableFaceCulling() {
	glEnable(GL_CULL_FACE);
}

void RD_RenderingAPI_GL::DisableFaceCulling() {
	glDisable(GL_CULL_FACE);
}

//---------------------------------------------  RD_RenderingAPI_VertexBufferInstancedGL ---------------------------------------------  

RD_RenderingAPI_VertexBufferInstancedGL::RD_RenderingAPI_VertexBufferInstancedGL() :
	RD_RenderingAPI_VertexBufferGL() {
	m_dataSize = 1;
}

RD_RenderingAPI_VertexBufferInstancedGL::~RD_RenderingAPI_VertexBufferInstancedGL() {

}

void RD_RenderingAPI_VertexBufferInstancedGL::SetVertexAttr(float* data, DataTypes type, const int count, const int divisor, const int arrayID) {
	int size = 0;
	switch (type)
	{
	case TFLOAT:
		m_dataSize = 1;
		break;
	case TVEC2:
		m_dataSize = 2;
		break;
	case TVEC3:
		m_dataSize = 3;
		break;
	case TMAT4:
		m_dataSize = 16;
		break;
	default:
		m_dataSize = 1;
		break;
	}

	unsigned int vbuff = 0;
	glGenBuffers(1, &vbuff);
	m_attrVBO.push_back(vbuff);

	glBindBuffer(GL_ARRAY_BUFFER, m_attrVBO[arrayID]);
	glBufferData(GL_ARRAY_BUFFER, count * m_dataSize * sizeof(float), static_cast<void*>(data), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	if (type == DataTypes::TMAT4) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glEnableVertexAttribArray(arrayID + 3);
		glEnableVertexAttribArray(arrayID + 4);
		glEnableVertexAttribArray(arrayID + 5);
		glEnableVertexAttribArray(arrayID + 6);

		glBindBuffer(GL_ARRAY_BUFFER, m_attrVBO[arrayID]);

		glVertexAttribPointer(arrayID + 3, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(0));
		glVertexAttribPointer(arrayID + 4, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 4));
		glVertexAttribPointer(arrayID + 5, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 8));
		glVertexAttribPointer(arrayID + 6, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 12));

		glVertexAttribDivisor(arrayID + 3, divisor);
		glVertexAttribDivisor(arrayID + 4, divisor);
		glVertexAttribDivisor(arrayID + 5, divisor);
		glVertexAttribDivisor(arrayID + 6, divisor);

		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(arrayID + 3);
	glBindBuffer(GL_ARRAY_BUFFER, m_attrVBO[arrayID]);
	glVertexAttribPointer(arrayID + 3, m_dataSize, GL_FLOAT, GL_FALSE, m_dataSize * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribDivisor(3, divisor);
}

void RD_RenderingAPI_VertexBufferInstancedGL::UpdateBufferData(float* data, const int count, const int arrayID) {
	glBindBuffer(GL_ARRAY_BUFFER, m_attrVBO[arrayID]);
	glBufferData(GL_ARRAY_BUFFER, count * m_dataSize * sizeof(float), static_cast<void*>(data), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//---------------------------------------------  RD_RenderingAPI_VertexElemBufferGL  ---------------------------------------------

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

//---------------------------------------------  RD_RenderingAPI_VertexBufferGL  ---------------------------------------------

RD_RenderingAPI_VertexBufferGL::RD_RenderingAPI_VertexBufferGL() : RD_RenderingAPI_VertexBuffer() {
	VAO = 0;
	VBO = 0;

	float_count = 0;
}

RD_RenderingAPI_VertexBufferGL::~RD_RenderingAPI_VertexBufferGL() {
	DeleteBuffer();
}

void RD_RenderingAPI_VertexBufferGL::CreateBuffer() {
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
}

void RD_RenderingAPI_VertexBufferGL::FillBufferData(float* data, int count) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);

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

	float_count = count;
}

void RD_RenderingAPI_VertexBufferGL::BindBuffer() {
	glBindVertexArray(VAO);
}

void RD_RenderingAPI_VertexBufferGL::UnbindBuffer() {
	glBindVertexArray(0);
}

void RD_RenderingAPI_VertexBufferGL::DeleteBuffer() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

unsigned int RD_RenderingAPI_VertexBufferGL::GetFloatCount() {
	return float_count;
}

//---------------------------------------------  Misc  ---------------------------------------------
bool CheckExtensionAvailability(std::string ext) {
	int k = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &k);

	for (int i = 0; i < k; i++) {
		const GLubyte* kext_str = glGetStringi(GL_EXTENSIONS, i);

		if (!strcmp((const char*)kext_str, ext.c_str())) {
			return true;
		}
	}

	return false;
}

#endif //BUILD_OPENGL