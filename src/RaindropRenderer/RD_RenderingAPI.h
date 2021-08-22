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

#include <iostream>
#include <string>

#include "RD_Texture.h"
#include "RD_FrameBuffer.h"
#include "RD_ShaderLoader.h"
#include "RD_Cubemap.h"

//MASK PROPERTY
#define COLOR_BUFFER 1 //0b 0000 ... 0000 0001
#define DEPTH_BUFFER 2 //0b 0000 ... 0000 0010

enum DrawMode {
	TRIANGLES = 0,
	TRIANGLES_STRIP = 1
};

enum FillingMode {
	FILLED,
	WIREFRAME
};

class RD_API RD_RenderingAPI_VertexElemBuffer {
public:
	RD_RenderingAPI_VertexElemBuffer() {}
	virtual ~RD_RenderingAPI_VertexElemBuffer() {}

	virtual void CreateBuffer() = 0;
	virtual void FillBufferData(float* data, int count, unsigned int* elemsData, int elemCount) = 0;
	virtual void DeleteBuffer() = 0;

	virtual void BindBuffer() = 0;
	virtual void UnbindBuffer() = 0;

	virtual unsigned int GetElementCount() = 0;
};

class RD_API RD_RenderingAPI_VertexBuffer {
public:
	RD_RenderingAPI_VertexBuffer() {}
	virtual ~RD_RenderingAPI_VertexBuffer() {}

	virtual void CreateBuffer() = 0;
	virtual void FillBufferData(float* data, int count) = 0;
	virtual void DeleteBuffer() = 0;

	virtual void BindBuffer() = 0;
	virtual void UnbindBuffer() = 0;

	virtual unsigned int GetFloatCount() = 0;
};

enum DataTypes {
	TFLOAT,
	TVEC2,
	TVEC3,
	TMAT4
};

class RD_API RD_RenderingAPI_VertexBufferInstanced :
	public RD_RenderingAPI_VertexBuffer{
public:
	RD_RenderingAPI_VertexBufferInstanced() {}
	virtual ~RD_RenderingAPI_VertexBufferInstanced() {};

	virtual void SetVertexAttr(float* data, DataTypes type, const int count, const int divisor, const int arrayID) = 0;
	virtual void UpdateBufferData(float* data, const int count, const int arrayID) = 0;
};

class RD_API RD_WindowingSystem {
public:
	RD_WindowingSystem() {}
	virtual ~RD_WindowingSystem() {}

	virtual bool OpenWindow(std::string name, int w, int h) = 0;
	virtual void SetFullscreenMode(bool mode) = 0;

	virtual bool GetKeyPress(int key) = 0;
	virtual bool GetMouseButton(int button) = 0;

	virtual double GetCursorPosX() = 0;
	virtual double GetCursorPosY() = 0;
	virtual void SetCursorPos(double x, double y) = 0;

	virtual void CaptureCursor(bool mode) = 0;

	virtual int GetHeight() = 0;
	virtual int GetWidth() = 0;

	virtual void SwapWindow() = 0;
	virtual void SetVSync(const bool vsync) = 0;

	virtual bool WantToClose() = 0;
	virtual void PollEvents() = 0;

protected:
	int m_w, m_h;
};

class RD_API RD_RenderingAPI
{
public:
	RD_RenderingAPI() {}
	virtual ~RD_RenderingAPI() {}

	virtual bool InitializeAPI(int w, int h, std::string wname) = 0;
	virtual RD_WindowingSystem* GetWindowingSystem() = 0;

	virtual RD_RenderingAPI_VertexElemBuffer* CreateVertexElemBuffer() = 0;
	virtual RD_RenderingAPI_VertexBuffer* CreateVertexBuffer() = 0;
	virtual RD_RenderingAPI_VertexBufferInstanced* CreateVertexBufferInstanced() = 0;
	virtual RD_Texture* CreateTexture() = 0;
	virtual RD_FrameBuffer* CreateFrameBuffer(int w, int h, bool nodepth) = 0;
	virtual RD_ShaderLoader* CreateShader() = 0;
	virtual RD_Cubemap* CreateCubemap() = 0;
	virtual RD_UniformBuffer* CreateUniformBuffer(const size_t size, const int binding) = 0;
	virtual RD_ShaderStorageBuffer* CreateShaderStorageBuffer(const size_t size, const int binding) = 0;

	virtual void SetViewportSize(int w, int h, int x, int y) = 0;

	virtual void Clear(int masks) = 0;
	virtual void SetClearColor(const vec3f& color = vec3f()) = 0;
	virtual void Draw(RD_RenderingAPI_VertexElemBuffer* vbuff) = 0;
	virtual void DrawVB(RD_RenderingAPI_VertexBuffer* vbuff, DrawMode dm) = 0;
	virtual void DrawInstanced(RD_RenderingAPI_VertexBufferInstanced* vbuff, const int nbr, DrawMode dm) = 0;
	virtual void SetFilledMode(FillingMode fmode) = 0;

	virtual void EnableFaceCulling() = 0;
	virtual void DisableFaceCulling() = 0;

	virtual int GetMaxTextureCount() = 0;
	virtual bool AreBindlessTexturesAvailable() = 0;

	virtual API GetAPIType() = 0;
};