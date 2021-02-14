#include "pch.h"
#include "RD_RenderingAPI_DX11.h"

#ifdef BUILD_D3D11

RD_WindowingSystemWin32::RD_WindowingSystemWin32(RaindropRenderer* rndr) {
	m_wantToClose = false;
}

RD_WindowingSystemWin32::~RD_WindowingSystemWin32() {

}

bool RD_WindowingSystemWin32::OpenWindow(std::string name, int w, int h) {
	m_hInst = (HINSTANCE)GetModuleHandle(NULL);

	WCHAR exePath[MAX_PATH];
	GetModuleFileName(NULL, exePath, MAX_PATH);

	assert(m_hInst != NULL);

	HICON hIcon = ExtractIcon(m_hInst, exePath, 0);

	WNDCLASS wClass;
	wClass.style = CS_DBLCLKS;
	wClass.lpfnWndProc = RD_WindowingSystemWin32::WndCallback;
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;
	wClass.hInstance = m_hInst;
	wClass.hIcon = hIcon;
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = L"EXPGE_WINDOW";

	if (!RegisterClass(&wClass)) {
		DWORD dwError = GetLastError();
		if (dwError != ERROR_CLASS_ALREADY_EXISTS) {
			return false;
		}
	}

	m_hWnd = CreateWindowEx(
		0,
		L"EXPGE_WINDOW",
		StrToWStr(name).c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		w,
		h,
		NULL,
		NULL,
		m_hInst,
		NULL);

	if (m_hWnd == NULL) {
		return false;
	}

	ShowWindow(m_hWnd, true);

	return true;
}

void RD_WindowingSystemWin32::SetFullscreenMode(bool mode) {

}

int RD_WindowingSystemWin32::GetHeight() {
	return 0;
}

int RD_WindowingSystemWin32::GetWidth() {
	return 0;
}

void RD_WindowingSystemWin32::SwapWindow() {

}

bool RD_WindowingSystemWin32::WantToClose() {
	return m_wantToClose;
}

bool RD_WindowingSystemWin32::GetKeyPress(int key) {
	return false;
}

bool RD_WindowingSystemWin32::GetMouseButton(int button) {
	return false;
}

double RD_WindowingSystemWin32::GetCursorPosX() {
	return 0.0;
}

double RD_WindowingSystemWin32::GetCursorPosY() {
	return 0.0;
}

void RD_WindowingSystemWin32::SetCursorPos(double x, double y) {

}

void RD_WindowingSystemWin32::PollEvents() {
	MSG msg;
	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	if (msg.message == WM_QUIT) {
		m_wantToClose = true;
	}
}

void RD_WindowingSystemWin32::CaptureCursor(const bool mode) {

}

void RD_WindowingSystemWin32::SetVSync(const bool vsync) {

}

//------------------ RD_RenderingAPI_DX11 --------------------

RD_RenderingAPI_DX11::RD_RenderingAPI_DX11(RaindropRenderer* rndr) {
	m_winsys = new RD_WindowingSystemWin32(rndr);
}

RD_RenderingAPI_DX11::~RD_RenderingAPI_DX11() {
	delete m_winsys;

	m_maintarget->Release();
	m_backbuffer->Release();

	m_swapChain->Release();
	m_immContext->Release();
	m_device->Release();
}

bool RD_RenderingAPI_DX11::InitializeAPI(int w, int h, std::string wname) {
	if (!m_winsys->OpenWindow(wname, w, h)) {
		dispErrorMessageBox(StrToWStr("Cannot open the main window."));
		return false;
	}

	CreateDeviceSwapChain();

	return true;
}

void RD_RenderingAPI_DX11::CreateDeviceSwapChain() {
	DXGI_SWAP_CHAIN_DESC swapChaind;
	ZeroMemory(&swapChaind, sizeof(swapChaind));
	swapChaind.BufferCount = 1;
	swapChaind.BufferDesc.Width = m_winsys->GetWidth();
	swapChaind.BufferDesc.Height = m_winsys->GetHeight();
	swapChaind.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChaind.BufferDesc.RefreshRate.Numerator = 60;
	swapChaind.BufferDesc.RefreshRate.Denominator = 1;
	swapChaind.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChaind.OutputWindow = m_winsys->GetWin32Window();
	swapChaind.SampleDesc.Count = 1;
	swapChaind.SampleDesc.Quality = 0;
	swapChaind.Windowed = TRUE;

	D3D_FEATURE_LEVEL ftrLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL ftrLevel;

	HRESULT res = S_OK;
	res = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&ftrLevels,
		1,
		D3D11_SDK_VERSION,
		&swapChaind,
		&m_swapChain,
		&m_device,
		&ftrLevel,
		&m_immContext);

	if (FAILED(res)) {
		dispErrorMessageBox(StrToWStr("Cannot create D3D11Device and DXGISwapChain."));
		exit(-1);
	}
	else {
		std::cout << "Created device & swap chain." << std::endl;
	}

	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_backbuffer);
	if (!m_backbuffer) {
		dispErrorMessageBox(StrToWStr("Cannot create back buffer."));
		return;
	}

	m_device->CreateRenderTargetView(m_backbuffer, NULL, &m_maintarget);

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Width = m_winsys->GetWidth();
	vp.Height = m_winsys->GetHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_immContext->RSSetViewports(1, &vp);
}

RD_WindowingSystem* RD_RenderingAPI_DX11::GetWindowingSystem() {
	return m_winsys;
}

RD_RenderingAPI_VertexElemBuffer* RD_RenderingAPI_DX11::CreateVertexElemBuffer() {
	return new RD_RenderingAPI_VertexElemBufferDX11();
}

RD_RenderingAPI_VertexBuffer* RD_RenderingAPI_DX11::CreateVertexBuffer() {
	return new RD_RenderingAPI_VertexBufferDX11();
}

RD_Texture* RD_RenderingAPI_DX11::CreateTexture() {
	return nullptr;
}

RD_FrameBuffer* RD_RenderingAPI_DX11::CreateFrameBuffer(int w, int h, bool nodepth) {
	return nullptr;
}

RD_ShaderLoader* RD_RenderingAPI_DX11::CreateShader() {
	return new RD_ShaderLoader_DX11(m_device);
}

RD_Cubemap* RD_RenderingAPI_DX11::CreateCubemap() {
	return nullptr;
}

RD_UniformBuffer* RD_RenderingAPI_DX11::CreateUniformBuffer(const size_t size, const int binding) {
	return nullptr;
}

void RD_RenderingAPI_DX11::SetViewportSize(int w, int h, int x, int y) {

}

void RD_RenderingAPI_DX11::Draw(RD_RenderingAPI_VertexElemBuffer* vbuff) {

}

void RD_RenderingAPI_DX11::DrawVB(RD_RenderingAPI_VertexBuffer* vbuff, DrawMode dm) {

}

void RD_RenderingAPI_DX11::SetFilledMode(FillingMode fmode) {

}

void RD_RenderingAPI_DX11::Clear(int mask) {

}

void RD_RenderingAPI_DX11::SetClearColor(const vec3f& color) {

}

int RD_RenderingAPI_DX11::GetMaxTextureCount() {
	return 0;
}

#endif // BUILD_D3D11
