#pragma once

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RD_DLLAPI __declspec(dllexport)
#else
#define RD_DLLAPI __declspec(dllimport)
#endif // RAINDROPRENDEREREXPORTS
#else
#define RD_DLLAPI
#endif // _WIN32

#include <iostream>
#include <memory>
#include <optional>

#include "RD_RenderPass.h"
#include "RD_Pipeline.h"
#include "RD_ShaderLoader.h"
#include "RD_Buffer.h"
#include "RD_UniformBuffer.h"
#include "RD_Texture.h"
#include "RD_Callback.h"
#include "vec.h"

class RD_DLLAPI RD_Windowing {
public:
	RD_Windowing() {};
	virtual ~RD_Windowing() {};

	virtual bool OpenWindow(std::string name, const int w, const int h) = 0;
	virtual bool WantToClose() = 0;

	virtual bool ResizeFrame(const int w, const int h) = 0;
    virtual void SetExternalResizeCallback(std::shared_ptr<RD_Callback> cbck) = 0;
	virtual bool WasResized() = 0;

	virtual int GetHeight() = 0;
	virtual int GetWidth() = 0;

	virtual void Present() = 0;
    virtual void BuildBlitPipeline(std::string enginePath) = 0;

    virtual void SetPresentTexture(std::shared_ptr<RD_Texture> tex) = 0;

	virtual void PollEvents() = 0;
};

class RD_DLLAPI RD_API
{
public:
	RD_API() {};
	virtual ~RD_API() {};

	virtual bool InitAPI(std::string name, const int w, const int h) = 0;
	virtual bool MakeFramebuffers() = 0;

	virtual void ProperQuit() = 0;

	virtual std::shared_ptr<RD_ShaderLoader> CreateShader() = 0;
	virtual std::shared_ptr<RD_RenderPass> CreateRenderPass(std::vector<RD_Attachment> attachments, float width, float height) = 0;
	virtual std::shared_ptr<RD_Pipeline> CreatePipeline(std::shared_ptr<RD_RenderPass> rpass, std::shared_ptr<RD_ShaderLoader> shader, bool extSignaling = false) = 0;
    virtual std::shared_ptr<RD_VertexBuffer> CreateVertexBuffer() = 0;
    virtual std::shared_ptr<RD_IndexedVertexBuffer> CreateIndexedVertexBuffer() = 0;
    virtual std::shared_ptr<RD_UniformBuffer> CreateUniformBuffer(uint32_t binding) = 0;
    virtual std::shared_ptr<RD_Texture> CreateTexture() = 0;

	virtual std::shared_ptr<RD_Windowing> GetWindowingSystem() = 0;
};

#ifdef BUILD_VULKAN

#include <vector>
#include <optional>
#include <limits>
#include <algorithm>
#include <set>
#include <cassert>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

bool CheckDeviceExtensionSupport(VkPhysicalDevice dev);

struct RD_DLLAPI RD_SwapChainDetails {
	VkSurfaceCapabilitiesKHR cap;
	std::vector<VkSurfaceFormatKHR> fmt;
	std::vector<VkPresentModeKHR> pmode;
};

struct RD_DLLAPI QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentationFamily;

	bool IsComplete(VkPhysicalDevice dev) {
		return graphicsFamily.has_value() && presentationFamily.has_value() && CheckDeviceExtensionSupport(dev);
	}
};

class RD_DLLAPI RD_Windowing_GLFW : public RD_Windowing {
public:
	RD_Windowing_GLFW(RD_API* api);
	~RD_Windowing_GLFW() override;

	void CleanupVk(VkInstance inst, VkDevice dev, bool surfaceNoDelete = false);

	bool OpenWindow(std::string name, const int w, const int h) override;
	bool WantToClose() override;

	bool ResizeFrame(const int w, const int h) override;
    void SetExternalResizeCallback(std::shared_ptr<RD_Callback> cbck) override;
	bool WasResized() override;

    void SetPresentTexture(std::shared_ptr<RD_Texture> tex) override;

	VkResult CreateWindowSurface(VkInstance inst);
	RD_SwapChainDetails QuerySwapchainSupport(VkPhysicalDevice dev);
	VkSurfaceFormatKHR ChooseSwapFormat(const std::vector<VkSurfaceFormatKHR> availFmt);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availPmodes);
	VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR& cap);
	bool CreateSwapChain(VkDevice dev, VkPhysicalDevice pdev, QueueFamilyIndices ind);
	bool CreateImageViews(VkDevice dev);
	bool BuildSyncObjects(VkDevice dev);

	int GetHeight() override;
	int GetWidth() override;

	void Present() override;
    void BuildBlitPipeline(std::string enginePath) override;

	void PollEvents() override;

	int GetExtensionsCount();
	const char** GetExtensionsNames();

	GLFWwindow* GetWindow();
	VkSurfaceKHR GetSurfaceHandle();

	bool MakeFramebuffers(VkDevice dev);

	void SendGraphicsQueue(VkQueue queue);
	void SendPresentQueue(VkQueue queue);
private:
	static void ResizeCBCK(GLFWwindow* win, int w, int h);

	GLFWwindow* m_win;
    RD_API* m_api;

	VkSurfaceKHR m_surface;
	VkSwapchainKHR m_swapChain;

	VkFormat m_scFormat;
	VkExtent2D m_scExtent;

	std::vector<VkImage> m_scImages;
	std::vector<VkImageView> m_scViews;
	std::vector<VkFramebuffer> m_scFbs;

	VkSemaphore m_imgAvail_s;
	VkSemaphore m_rndrFinished_s;
	VkFence m_inFLight_f;

	int m_w, m_h;
	uint32_t m_imgIdx;

	VkDevice m_dev;
	VkPhysicalDevice m_pdev;
	VkInstance m_inst;

	QueueFamilyIndices m_ind;

    std::shared_ptr<RD_RenderPass> m_rpass;
    std::shared_ptr<RD_Pipeline> m_pline;
    std::shared_ptr<RD_IndexedVertexBuffer> m_verticies;

	VkQueue m_gfxQueue;
	VkQueue m_presentQueue;

	bool m_resizedFlag;
    std::optional<std::shared_ptr<RD_Callback>> m_extCallback;
};

class RD_DLLAPI RD_API_Vk : public RD_API {
public:
	RD_API_Vk();
	~RD_API_Vk() override;

	bool InitAPI(std::string name, const int w, const int h) override;
	bool MakeFramebuffers() override;

    void ProperQuit() override;

	std::shared_ptr<RD_ShaderLoader> CreateShader() override;
	std::shared_ptr<RD_RenderPass> CreateRenderPass(std::vector<RD_Attachment> attachments, float width, float height) override;
	std::shared_ptr<RD_Pipeline> CreatePipeline(std::shared_ptr<RD_RenderPass> rpass, std::shared_ptr<RD_ShaderLoader> shader, bool extSignaling = false) override;
    std::shared_ptr<RD_VertexBuffer> CreateVertexBuffer() override;
    std::shared_ptr<RD_IndexedVertexBuffer> CreateIndexedVertexBuffer() override;
    std::shared_ptr<RD_UniformBuffer> CreateUniformBuffer(uint32_t binding) override;
    std::shared_ptr<RD_Texture> CreateTexture() override;

	std::shared_ptr<RD_Windowing> GetWindowingSystem() override;
private:
	bool CreateVkInst();
	bool CheckRequestedValidationLayers(std::vector<std::string> layers);
	bool SetupDebugMessenger();
	void PickPhysicalDevice();
	bool CreateDevice();
	bool CreateCommandPool();

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice dev);

	std::vector<const char*> GetRequiredExtensions();
	bool IsDeviceSuitable(VkPhysicalDevice dev);

	VkInstance m_inst;

	VkPhysicalDevice m_dev;
	VkDevice m_ldev;

	VkQueue m_gfx_queue;
	VkQueue m_present_queue;

	VkCommandPool m_pool;

	bool m_validationLayers;
	VkDebugUtilsMessengerEXT m_cbck_dbg;

	std::shared_ptr<RD_Windowing_GLFW> m_winsys;
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
