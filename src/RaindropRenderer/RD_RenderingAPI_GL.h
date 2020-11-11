#pragma once

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RD_API __declspec(dllexport)
#else
#define RD_API __declspec(dllimport)
#endif //RAINDROPRENDERER_EXPORTS
#else
#define RD_API
#endif //_WIN32

//#ifdef BUILD_OPENGL

#include "RD_RenderingAPI.h"

#include "RaindropRenderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <string>

class RD_API RD_WindowingSystemGLFW : public RD_WindowingSystem {
public:
	RD_WindowingSystemGLFW(RaindropRenderer* rndr);
	virtual ~RD_WindowingSystemGLFW();

	virtual bool OpenWindow(std::string name, int w, int h);
	virtual void SetFullscreenMode(bool mode);

	virtual int GetHeight();
	virtual int GetWidth();

	virtual void SwapWindow();
	virtual bool WantToClose();

	virtual bool GetKeyPress(int key);
	virtual bool GetMouseButton(int button);

	virtual double GetCursorPosX();
	virtual double GetCursorPosY();
	virtual void SetCursorPos(double x, double y);

	virtual void PollEvents();

	virtual void CaptureCursor(bool mode);

	GLFWwindow* GetWindow();

private:
	GLFWwindow* m_win;
	RaindropRenderer* m_rndr;
};

void glfwWinCallback(GLFWwindow* win, int w, int h);

class RD_API RD_RenderingAPI_VertexElemBufferGL : public RD_RenderingAPI_VertexElemBuffer {
public:
	RD_RenderingAPI_VertexElemBufferGL();
	virtual ~RD_RenderingAPI_VertexElemBufferGL();

	virtual void CreateBuffer();
	virtual void FillBufferData(float* data, int count, unsigned int* elemData, int elemCount);
	virtual void DeleteBuffer();

	virtual void BindBuffer();
	virtual void UnbindBuffer();

private:
	unsigned int VAO, VBO, EBO;
};


class RD_API RD_RenderingAPI_GL : public RD_RenderingAPI {
public:
	RD_RenderingAPI_GL(RaindropRenderer* rndr);
	virtual ~RD_RenderingAPI_GL();

	virtual bool InitializeAPI(int w, int h, std::string wname);
	virtual RD_WindowingSystem* GetWindowingSystem();

	virtual RD_RenderingAPI_VertexElemBufferGL* CreateVertexElemBuffer();
	virtual RD_Texture* CreateTexture();
	virtual RD_FrameBuffer* CreateFrameBuffer();

	virtual void Draw(RD_RenderingAPI_VertexElemBuffer* vbuff, DrawMode rndrMode);
	virtual void SetFilledMode(FillingMode fmode);

	virtual void Clear(int mask);

private:
	RD_WindowingSystemGLFW* m_win_sys;
};
//#endif //BUILD_OPENGL