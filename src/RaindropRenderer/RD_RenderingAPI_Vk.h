#pragma once
#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RD_API __declspec(dllexport)
	#else
		#define RD_API __declspec(dllimport)
	#endif //RAINDROPRENDERER_EXPORTS

	#define VK_USE_PLATFORM_WIN32_KHR
#else
	#define RD_API
#endif //_WIN32

#ifdef BUILD_VULKAN

#include <optional>
#include <set>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#ifdef _WIN32
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include <GLFW/glfw3native.h>
#endif //_WIN32

#include "RD_RenderingAPI.h"

class RD_API RD_WindowingSystemGLFW_Vk : public RD_WindowingSystem {
public:
	RD_WindowingSystemGLFW_Vk(RaindropRenderer* rndr);
	virtual ~RD_WindowingSystemGLFW_Vk();
	void CleanupVK(VkInstance inst);

	virtual bool OpenWindow(std::string name, int w, int h);
	VkResult CreateWindowSurface(VkInstance inst);
	virtual void SetFullscreenMode(bool mode);

	virtual int GetHeight();
	virtual int GetWidth();

	virtual void SwapWindow();
	virtual bool WantToClose();

	virtual bool GetKeyPress(int key);
	virtual bool GetKeyRelease(int key);
	virtual bool GetMouseButton(int button);

	virtual double GetCursorPosX();
	virtual double GetCursorPosY();
	virtual void SetCursorPos(double x, double y);

	virtual void PollEvents();

	virtual void CaptureCursor(bool mode);
	virtual void SetVSync(const bool vsync);

	int GetExtensionsCount();
	const char** GetExtensionsNames();

	GLFWwindow* GetWindow();
	VkSurfaceKHR GetSurfaceHandle();

private:
	GLFWwindow* m_win;
	RaindropRenderer* m_rndr;

	VkSurfaceKHR m_surface;
};

class RD_API RD_RenderingAPI_VertexElemBufferVk : public RD_RenderingAPI_VertexElemBuffer {
public:
	RD_RenderingAPI_VertexElemBufferVk();
	virtual ~RD_RenderingAPI_VertexElemBufferVk();

	virtual void CreateBuffer();
	virtual void FillBufferData(float* data, int count, unsigned int* elemData, int elemCount);
	virtual void DeleteBuffer();

	virtual void BindBuffer();
	virtual void UnbindBuffer();

	virtual unsigned int GetElementCount();

private:
	unsigned int elem_count;
};

class RD_API RD_RenderingAPI_VertexBufferVk : public RD_RenderingAPI_VertexBuffer {
public:
	RD_RenderingAPI_VertexBufferVk();
	virtual ~RD_RenderingAPI_VertexBufferVk();

	virtual void CreateBuffer();
	virtual void FillBufferData(float* data, int count);
	virtual void DeleteBuffer();

	virtual void BindBuffer();
	virtual void UnbindBuffer();

	virtual unsigned int GetFloatCount();
protected:
	unsigned int VAO, VBO, float_count;
};

class RD_API RD_RenderingAPI_VertexBufferInstancedVk :
	public RD_RenderingAPI_VertexBufferVk,
	public RD_RenderingAPI_VertexBufferInstanced {
public:
	RD_RenderingAPI_VertexBufferInstancedVk();
	virtual ~RD_RenderingAPI_VertexBufferInstancedVk();

	virtual void SetVertexAttr(float* data, DataTypes type, const int count, const int divisor, const int arrayID);
	virtual void UpdateBufferData(float* data, const int count, const int arrayID);

	virtual void CreateBuffer() override {
		RD_RenderingAPI_VertexBufferVk::CreateBuffer();
	}

	virtual void FillBufferData(float* data, int count) override {
		RD_RenderingAPI_VertexBufferVk::FillBufferData(data, count);
	}

	virtual void DeleteBuffer() override {
		RD_RenderingAPI_VertexBufferVk::DeleteBuffer();
	}

	virtual void BindBuffer() override {
		RD_RenderingAPI_VertexBufferVk::BindBuffer();
	}

	virtual void UnbindBuffer() override {
		RD_RenderingAPI_VertexBufferVk::UnbindBuffer();
	}

	virtual unsigned int GetFloatCount() override {
		return RD_RenderingAPI_VertexBufferVk::GetFloatCount();
	}

private:
	std::vector<unsigned int> m_attrVBO;
	unsigned int m_dataSize;
};

bool CheckDeviceExtensionSupport(VkPhysicalDevice dev);

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentationFamily;

	bool IsComplete(VkPhysicalDevice dev) {
		return graphicsFamily.has_value() && presentationFamily.has_value() && CheckDeviceExtensionSupport(dev);
	}
};

class RD_API RD_RenderingAPI_Vk : public RD_RenderingAPI {
public:
	RD_RenderingAPI_Vk(RaindropRenderer* rndr);
	virtual ~RD_RenderingAPI_Vk();

	virtual bool InitializeAPI(int w, int h, std::string wname);
	virtual RD_WindowingSystem* GetWindowingSystem();

	virtual RD_RenderingAPI_VertexElemBuffer* CreateVertexElemBuffer();
	virtual RD_RenderingAPI_VertexBuffer* CreateVertexBuffer();
	virtual RD_RenderingAPI_VertexBufferInstanced* CreateVertexBufferInstanced();
	virtual RD_Texture* CreateTexture();
	virtual RD_FrameBuffer* CreateFrameBuffer(int w, int h, bool nodepth = false);
	virtual RD_ShaderLoader* CreateShader();
	virtual RD_Cubemap* CreateCubemap();
	virtual RD_UniformBuffer* CreateUniformBuffer(const size_t bufferSize, const int binding);
	virtual RD_ShaderStorageBuffer* CreateShaderStorageBuffer(const size_t bufferSize, const int binding);

	virtual void SetViewportSize(int w, int h, int x, int y);

	virtual void Draw(RD_RenderingAPI_VertexElemBuffer* vbuff);
	virtual void DrawVB(RD_RenderingAPI_VertexBuffer* vbuff, DrawMode dm);
	virtual void DrawInstanced(RD_RenderingAPI_VertexBufferInstanced* vbuff, const int nbr, DrawMode dm);
	virtual void SetFilledMode(FillingMode fmode);

	virtual void Clear(int mask);
	virtual void SetClearColor(const vec3f& color);

	virtual void EnableFaceCulling();
	virtual void DisableFaceCulling();

	virtual int GetMaxTextureCount();
	virtual bool AreBindlessTexturesAvailable();

	virtual API GetAPIType() { return API::VULKAN; }

private:
	bool CreateVkInst();
	bool CheckRequestedValidationLayers(std::vector<std::string> layers);
	bool SetupDebugMessenger();
	void PickPhysicalDevice();
	bool CreateDevice();
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice dev);

	std::vector<const char*> GetRequiredExtensions();
	bool IsDeviceSuitable(VkPhysicalDevice dev);

	VkInstance m_inst;
	
	VkPhysicalDevice m_dev;
	VkDevice m_ldev;
	
	VkQueue m_gfx_queue;
	VkQueue m_present_queue;

	VkDebugUtilsMessengerEXT m_cbck_dbg;

	RD_WindowingSystemGLFW_Vk* m_win_sys;

	bool m_bindless_tex_available;
	bool m_validationLayers;
};

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {

	std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

#endif //BUILD_VULKAN