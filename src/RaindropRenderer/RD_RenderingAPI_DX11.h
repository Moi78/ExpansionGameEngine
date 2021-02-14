#pragma once

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RD_API __declspec(dllexport)
	#else
		#define RD_API __declspec(dllimport)
	#endif //RAINDROPRENDERER_EXPORTS

	#include <Windows.h>
	#include <shellapi.h>
	#include <WinUser.h>
	#include <windef.h>
	#include <comdef.h>

	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "dxgi.lib")
	#include <d3d11.h>
	#include <dxgi.h>
#else
	#define RD_API
#endif //_WIN32

#include "RaindropRenderer.h"
#include "RD_RenderingAPI.h"

#include <iostream>
#include <string>

#ifdef BUILD_D3D11

class RD_API RD_WindowingSystemWin32 : public RD_WindowingSystem {
public:
	RD_WindowingSystemWin32(RaindropRenderer* rndr);
	virtual ~RD_WindowingSystemWin32();

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

	virtual void CaptureCursor(const bool mode);
	virtual void SetVSync(const bool vsync);

	HWND GetWin32Window() { return m_hWnd; }

	static LRESULT CALLBACK WndCallback(HWND hwnd, UINT uMSG, WPARAM wParam, LPARAM lParam) {
		switch (uMSG)
		{
		case WM_CLOSE:
			return 0;
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hwnd, uMSG, wParam, lParam);
	}

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	bool m_wantToClose;
};

class RD_API RD_RenderingAPI_VertexBufferDX11 : public RD_RenderingAPI_VertexBuffer {
public:
	RD_RenderingAPI_VertexBufferDX11() {};
	virtual ~RD_RenderingAPI_VertexBufferDX11() {};

	virtual void CreateBuffer() {};
	virtual void FillBufferData(float* data, int count) {};
	virtual void DeleteBuffer() {};

	virtual void BindBuffer() {};
	virtual void UnbindBuffer() {};

	virtual unsigned int GetFloatCount() { return 0; };
};

class RD_API RD_RenderingAPI_VertexElemBufferDX11 : public RD_RenderingAPI_VertexElemBuffer {
public:
	RD_RenderingAPI_VertexElemBufferDX11() {};
	virtual ~RD_RenderingAPI_VertexElemBufferDX11() {};

	virtual void CreateBuffer() {};
	virtual void FillBufferData(float* data, int count, unsigned int* elemData, int elemCount) {};
	virtual void DeleteBuffer() {};

	virtual void BindBuffer() {};
	virtual void UnbindBuffer() {};

	virtual unsigned int GetElementCount() { return 0; };
};

class RD_API RD_RenderingAPI_DX11 : public RD_RenderingAPI {
public:
	RD_RenderingAPI_DX11(RaindropRenderer* rndr);
	virtual ~RD_RenderingAPI_DX11();

	virtual bool InitializeAPI(int w, int h, std::string wname);
	virtual RD_WindowingSystem* GetWindowingSystem();

	virtual RD_RenderingAPI_VertexElemBuffer* CreateVertexElemBuffer();
	virtual RD_RenderingAPI_VertexBuffer* CreateVertexBuffer();
	virtual RD_Texture* CreateTexture();
	virtual RD_FrameBuffer* CreateFrameBuffer(int w, int h, bool nodepth = false);
	virtual RD_ShaderLoader* CreateShader();
	virtual RD_Cubemap* CreateCubemap();
	virtual RD_UniformBuffer* CreateUniformBuffer(const size_t size, const int binding);

	virtual void SetViewportSize(int w, int h, int x, int y);

	virtual void Draw(RD_RenderingAPI_VertexElemBuffer* vbuff);
	virtual void DrawVB(RD_RenderingAPI_VertexBuffer* vbuff, DrawMode dm);
	virtual void SetFilledMode(FillingMode fmode);

	virtual void Clear(int mask);
	virtual void SetClearColor(const vec3f& color);

	virtual int GetMaxTextureCount();

private:
	void CreateDeviceSwapChain();

	RD_WindowingSystemWin32* m_winsys;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_immContext;
	IDXGISwapChain* m_swapChain;

	ID3D11Texture2D* m_backbuffer;
	ID3D11RenderTargetView* m_maintarget;
};

#endif // BUILD_D3D11