#include "pch.h"
#include "RD_RenderingAPI_Vk.h"

//---------------------------------------

std::vector<const char*> requestedExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// ------------ Vulkan Funcs ------------

VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

// ------------ RD_WindowingSystemGLFW_Vk -----------

RD_WindowingSystemGLFW_Vk::RD_WindowingSystemGLFW_Vk(RaindropRenderer* rndr) {
	m_rndr = rndr;
}

RD_WindowingSystemGLFW_Vk::~RD_WindowingSystemGLFW_Vk() {
	glfwDestroyWindow(m_win);
	glfwTerminate();
}

void RD_WindowingSystemGLFW_Vk::CleanupVK(VkInstance inst, VkDevice dev) {
	vkDestroySwapchainKHR(dev, m_swapChain, nullptr);
	vkDestroySurfaceKHR(inst, m_surface, nullptr);
}

bool RD_WindowingSystemGLFW_Vk::OpenWindow(std::string name, int w, int h) {
	if (!glfwInit()) {
		std::cout << "ERROR: Failed to init. GLFW." << std::endl;
		return false;
	}

	m_h = h;
	m_w = w;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_win = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr);

	return true;
}

void RD_WindowingSystemGLFW_Vk::SetFullscreenMode(bool mode) {
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

int RD_WindowingSystemGLFW_Vk::GetHeight() {
	glfwGetFramebufferSize(m_win, NULL, &m_h);
	return m_h;
}

int RD_WindowingSystemGLFW_Vk::GetWidth() {
	glfwGetFramebufferSize(m_win, &m_w, NULL);
	return m_w;
}

void RD_WindowingSystemGLFW_Vk::SwapWindow() {

}

bool RD_WindowingSystemGLFW_Vk::WantToClose() {
	return glfwWindowShouldClose(m_win);
}

bool RD_WindowingSystemGLFW_Vk::GetKeyPress(int key) {
	return glfwGetKey(m_win, key) == GLFW_PRESS;
}

bool RD_WindowingSystemGLFW_Vk::GetKeyRelease(int key) {
	return glfwGetKey(m_win, key) == GLFW_RELEASE;
}

bool RD_WindowingSystemGLFW_Vk::GetMouseButton(int button) {
	return glfwGetMouseButton(m_win, button) == GLFW_PRESS;
}

double RD_WindowingSystemGLFW_Vk::GetCursorPosX() {
	double xaxis;
	glfwGetCursorPos(m_win, &xaxis, NULL);

	return xaxis;
}

double RD_WindowingSystemGLFW_Vk::GetCursorPosY() {
	double yaxis;
	glfwGetCursorPos(m_win, NULL, &yaxis);

	return yaxis;
}

void RD_WindowingSystemGLFW_Vk::SetCursorPos(double x, double y) {
	glfwSetCursorPos(m_win, x, y);
}

void RD_WindowingSystemGLFW_Vk::PollEvents() {
	glfwPollEvents();
}

void RD_WindowingSystemGLFW_Vk::CaptureCursor(bool mode) {
	if (mode) {
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void RD_WindowingSystemGLFW_Vk::SetVSync(const bool vsync) {

}

GLFWwindow* RD_WindowingSystemGLFW_Vk::GetWindow() {
	return m_win;
}

int RD_WindowingSystemGLFW_Vk::GetExtensionsCount() {
	uint32_t count = 0;
	glfwGetRequiredInstanceExtensions(&count);

	return count;
}

const char** RD_WindowingSystemGLFW_Vk::GetExtensionsNames() {
	uint32_t count;
	return glfwGetRequiredInstanceExtensions(&count);
}

VkResult RD_WindowingSystemGLFW_Vk::CreateWindowSurface(VkInstance inst) {
	assert(m_win != nullptr && "A window must be opened to create a surface");

	VkWin32SurfaceCreateInfoKHR cInfo{};
	cInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	cInfo.hwnd = glfwGetWin32Window(m_win);
	cInfo.hinstance = GetModuleHandle(nullptr);

	VkResult res = vkCreateWin32SurfaceKHR(inst, &cInfo, nullptr, &m_surface);
	return res;
}

VkSurfaceKHR RD_WindowingSystemGLFW_Vk::GetSurfaceHandle() {
	return m_surface;
}

RD_SwapChainDetails RD_WindowingSystemGLFW_Vk::QuerySwapchainSupport(VkPhysicalDevice dev) {
	assert(m_surface != VK_NULL_HANDLE && "Surface must be initialized.");

	RD_SwapChainDetails details{};

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(dev, m_surface, &details.cap);

	uint32_t fmt_count;
	vkGetPhysicalDeviceSurfaceFormatsKHR(dev, m_surface, &fmt_count, nullptr);

	if (fmt_count != 0) {
		details.fmt.resize(fmt_count);
		vkGetPhysicalDeviceSurfaceFormatsKHR(dev, m_surface, &fmt_count, details.fmt.data());
	}

	uint32_t pres_mode_count;
	vkGetPhysicalDeviceSurfacePresentModesKHR(dev, m_surface, &pres_mode_count, nullptr);

	if (pres_mode_count != 0) {
		details.pmode.resize(pres_mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(dev, m_surface, &pres_mode_count, details.pmode.data());
	}

	return details;
}

VkSurfaceFormatKHR RD_WindowingSystemGLFW_Vk::ChooseSwapFormat(const std::vector<VkSurfaceFormatKHR> availFmt) {
	for (const auto& fmt : availFmt) {
		if (fmt.format == VK_FORMAT_R8G8B8_SRGB && fmt.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
			return fmt;
		}
	}

	return availFmt[0]; // If the format that I want is not listed...
}

VkPresentModeKHR RD_WindowingSystemGLFW_Vk::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availPmodes) {
	for (auto& pmode : availPmodes) {
		if (pmode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return pmode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

#undef max
VkExtent2D RD_WindowingSystemGLFW_Vk::ChooseSwapExtent(VkSurfaceCapabilitiesKHR& cap) {
	if (cap.currentExtent.width != std::numeric_limits<uint32_t>::max() && cap.currentExtent.height != std::numeric_limits<uint32_t>::max()) {
		return cap.currentExtent;
	}
	else {
		int w, h;
		h = GetHeight();
		w = GetWidth();

		VkExtent2D ext = {
			w,
			h
		};

		ext.height = std::clamp(ext.height, cap.minImageExtent.height, cap.maxImageExtent.height);
		ext.width = std::clamp(ext.width, cap.minImageExtent.width, cap.maxImageExtent.width);

		return ext;
	}
}

bool RD_WindowingSystemGLFW_Vk::CreateSwapChain(VkDevice dev, VkPhysicalDevice pdev, QueueFamilyIndices ind) {
	RD_SwapChainDetails swDet = QuerySwapchainSupport(pdev);

	VkSurfaceFormatKHR sfFormat = ChooseSwapFormat(swDet.fmt);
	VkPresentModeKHR presMode = ChooseSwapPresentMode(swDet.pmode);
	VkExtent2D extent = ChooseSwapExtent(swDet.cap);

	uint32_t imgCount = swDet.cap.minImageCount + 1;
	if (swDet.cap.maxImageCount > 0 && imgCount > swDet.cap.maxImageCount) {
		imgCount = swDet.cap.maxImageCount;
	}

	VkSwapchainCreateInfoKHR cInfo{};
	cInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	cInfo.surface = m_surface;
	cInfo.minImageCount = imgCount;
	cInfo.imageFormat = sfFormat.format;
	cInfo.imageColorSpace = sfFormat.colorSpace;
	cInfo.imageExtent = extent;
	cInfo.imageArrayLayers = 1;
	cInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t qFamInd[] = { ind.graphicsFamily.value(), ind.presentationFamily.value() };
	if (ind.graphicsFamily != ind.presentationFamily) {
		cInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		cInfo.queueFamilyIndexCount = 2;
		cInfo.pQueueFamilyIndices = qFamInd;
	}
	else {
		cInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		cInfo.queueFamilyIndexCount = 0;
		cInfo.pQueueFamilyIndices = nullptr;
	}

	cInfo.preTransform = swDet.cap.currentTransform;
	cInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	cInfo.presentMode = presMode;
	cInfo.clipped = VK_TRUE;
	cInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(dev, &cInfo, nullptr, &m_swapChain) == VK_SUCCESS) {
		return true;
	}
	else {
		return false;
	}
}

// ---------- RD_RenderingAPI_VertexElemBufferVk -----------

RD_RenderingAPI_VertexElemBufferVk::RD_RenderingAPI_VertexElemBufferVk() {
	elem_count = 0;
}

RD_RenderingAPI_VertexElemBufferVk::~RD_RenderingAPI_VertexElemBufferVk() {

}

void RD_RenderingAPI_VertexElemBufferVk::CreateBuffer() {

}

void RD_RenderingAPI_VertexElemBufferVk::FillBufferData(float* data, int count, unsigned int* elemData, int elemCount) {

}

void RD_RenderingAPI_VertexElemBufferVk::DeleteBuffer() {
	
}

void RD_RenderingAPI_VertexElemBufferVk::BindBuffer() {

}

void RD_RenderingAPI_VertexElemBufferVk::UnbindBuffer() {

}

unsigned int RD_RenderingAPI_VertexElemBufferVk::GetElementCount() {
	return elem_count;
}

// ----------- RD_RenderingAPI_VertexBufferVk ------------

RD_RenderingAPI_VertexBufferVk::RD_RenderingAPI_VertexBufferVk() {

}

RD_RenderingAPI_VertexBufferVk::~RD_RenderingAPI_VertexBufferVk() {

}

void RD_RenderingAPI_VertexBufferVk::CreateBuffer() {

}

void RD_RenderingAPI_VertexBufferVk::FillBufferData(float* data, int count) {

}

void RD_RenderingAPI_VertexBufferVk::DeleteBuffer() {

}

void RD_RenderingAPI_VertexBufferVk::BindBuffer() {

}

void RD_RenderingAPI_VertexBufferVk::UnbindBuffer() {

}

unsigned int RD_RenderingAPI_VertexBufferVk::GetFloatCount() {
	return 0;
}

// ----------- RD_RenderingAPI_VertexBufferInstancedVk ------------

RD_RenderingAPI_VertexBufferInstancedVk::RD_RenderingAPI_VertexBufferInstancedVk() {

}

RD_RenderingAPI_VertexBufferInstancedVk::~RD_RenderingAPI_VertexBufferInstancedVk() {

}

void RD_RenderingAPI_VertexBufferInstancedVk::SetVertexAttr(float* data, DataTypes type, const int count, const int divisor, const int arrayID) {

}

void RD_RenderingAPI_VertexBufferInstancedVk::UpdateBufferData(float* data, const int count, const int arrayID) {

}

// ---------- RD_RenderingAPI_Vk ----------
RD_RenderingAPI_Vk::RD_RenderingAPI_Vk(RaindropRenderer* rndr) {
	m_win_sys = new RD_WindowingSystemGLFW_Vk(rndr);

	m_validationLayers = true;
}

RD_RenderingAPI_Vk::~RD_RenderingAPI_Vk() {
	m_win_sys->CleanupVK(m_inst, m_ldev);

	vkDestroyDevice(m_ldev, nullptr);
	if (m_validationLayers) {
		vkDestroyDebugUtilsMessengerEXT(m_inst, m_cbck_dbg, nullptr);
	}

	vkDestroyInstance(m_inst, nullptr);

	delete m_win_sys;
}

bool RD_RenderingAPI_Vk::InitializeAPI(int w, int h, std::string wname) {
	m_win_sys->OpenWindow(wname, w, h);

	if (!CreateVkInst()) {
		return false;
	}

	if (m_validationLayers) {
		if (!SetupDebugMessenger()) {
			return false;
		}
	}

	if (m_win_sys->CreateWindowSurface(m_inst) != VK_SUCCESS) {
		dispErrorMessageBox(L"Failed to create window surface.");
		return false;
	}

	PickPhysicalDevice();

	if (!CreateDevice()) {
		return false;
	}

	QueueFamilyIndices qFamInd = FindQueueFamilies(m_dev);
	if (!m_win_sys->CreateSwapChain(m_ldev, m_dev, qFamInd)) {
		dispErrorMessageBox(L"Failed to create swapchain.");
		return false;
	}

	return true;
}

void RD_RenderingAPI_Vk::PickPhysicalDevice() {
	m_dev = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_inst, &deviceCount, nullptr);
	if (deviceCount == 0) {
		dispErrorMessageBox(L"No Vulkan capable GPU found.");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_inst, &deviceCount, devices.data());

	if (deviceCount == 1) {
		m_dev = devices[0];
		return;
	}

	for (auto dev : devices) {
		if (IsDeviceSuitable(dev)) {
			m_dev = dev;
			break;
		}
	}

	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(m_dev, &prop);
	std::cout << "Usign GPU : " << prop.deviceName << std::endl;
}

bool RD_RenderingAPI_Vk::CreateDevice() {
	QueueFamilyIndices ind = FindQueueFamilies(m_dev);

	std::vector<VkDeviceQueueCreateInfo> devCInfo;
	std::set<uint32_t> uniqueQFam = { ind.graphicsFamily.value(), ind.presentationFamily.value() };

	float prio = 1.0f;
	for (uint32_t qFam : uniqueQFam) {
		VkDeviceQueueCreateInfo cInfo{};
		cInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		cInfo.queueCount = 1;
		cInfo.queueFamilyIndex = qFam;
		cInfo.pQueuePriorities = &prio;
		devCInfo.push_back(cInfo);
	}

	VkPhysicalDeviceFeatures features{};

	VkDeviceCreateInfo devcInfo{};
	devcInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	devcInfo.pQueueCreateInfos = devCInfo.data();
	devcInfo.queueCreateInfoCount = static_cast<uint32_t>(devCInfo.size());
	devcInfo.pEnabledFeatures = &features;
	devcInfo.enabledExtensionCount = static_cast<uint32_t>(requestedExtensions.size());
	devcInfo.ppEnabledExtensionNames = requestedExtensions.data();

	VkResult res = vkCreateDevice(m_dev, &devcInfo, nullptr, &m_ldev);
	if (res != VK_SUCCESS) {
		dispErrorMessageBox(L"ERROR: Failed to create a logical device.");
		return false;
	}

	vkGetDeviceQueue(m_ldev, ind.graphicsFamily.value(), 0, &m_gfx_queue);
	vkGetDeviceQueue(m_ldev, ind.presentationFamily.value(), 0, &m_present_queue);

	return true;
}

QueueFamilyIndices RD_RenderingAPI_Vk::FindQueueFamilies(VkPhysicalDevice dev) {
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(dev, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, m_win_sys->GetSurfaceHandle(), &presSupport);
		if (presSupport) {
			indices.presentationFamily = i;
		}

		i++;
	}

	return indices;
}

bool RD_RenderingAPI_Vk::IsDeviceSuitable(VkPhysicalDevice dev) {
	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(dev, &prop);

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(dev, &features);

	QueueFamilyIndices qFam = FindQueueFamilies(dev);

	RD_SwapChainDetails scDet{};
	scDet = m_win_sys->QuerySwapchainSupport(dev);

	bool isSwapchainSuitable = !(scDet.fmt.empty()) && !(scDet.pmode.empty());

	return (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && qFam.IsComplete(dev) && isSwapchainSuitable;
}

bool RD_RenderingAPI_Vk::CreateVkInst() {
	std::vector<std::string> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	if (m_validationLayers && !CheckRequestedValidationLayers(validationLayers)) {
		std::cerr << "ERROR: Requested validation layers not available." << std::endl;

		return false;
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.apiVersion = VK_API_VERSION_1_3;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pApplicationName = "EXPGE Game";
	appInfo.pEngineName = "Expansion Game Engine";;
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	VkInstanceCreateInfo cInfo{};
	cInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	cInfo.pApplicationInfo = &appInfo;

	auto extensions = GetRequiredExtensions();
	cInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	cInfo.ppEnabledExtensionNames = extensions.data();

	std::vector<const char*> layersC;
	if (m_validationLayers) {
		cInfo.enabledLayerCount = validationLayers.size();

		for (auto& l : validationLayers) {
			layersC.push_back(l.c_str());
		}
		cInfo.ppEnabledLayerNames = layersC.data();
	}
	else {
		cInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateInstance(&cInfo, nullptr, &m_inst);
	if (result != VK_SUCCESS) {
		dispErrorMessageBox(L"Failed to create VkInstance.");
		return false;
	}

	return true;
}

bool RD_RenderingAPI_Vk::SetupDebugMessenger() {
	VkDebugUtilsMessengerCreateInfoEXT cInfo{};
	cInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	cInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	cInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	cInfo.pfnUserCallback = &DebugCallback;
	cInfo.pUserData = nullptr;

	VkResult res = vkCreateDebugUtilsMessengerEXT(m_inst, &cInfo, nullptr, &m_cbck_dbg);
	if (res != VK_SUCCESS) {
		std::cerr << "ERROR: Could not setup the Debug Messenger" << std::endl;
		return false;
	}

	return true;
}

std::vector<const char*> RD_RenderingAPI_Vk::GetRequiredExtensions() {
	uint32_t extCount;
	const char** extNames;
	extNames = glfwGetRequiredInstanceExtensions(&extCount);

	std::vector<const char*> requiredExtensions(extNames, extNames + extCount);
	if (m_validationLayers) {
		requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return requiredExtensions;
}

bool RD_RenderingAPI_Vk::CheckRequestedValidationLayers(std::vector<std::string> reqLayers) {
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> layerProp(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layerProp.data());

	for (auto layer : reqLayers) {
		bool layerFound = false;
		for (const auto& prop : layerProp) {
			if (std::string(prop.layerName) == layer) {
				layerFound = true;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

RD_WindowingSystem* RD_RenderingAPI_Vk::GetWindowingSystem() {
	return m_win_sys;
}

RD_RenderingAPI_VertexElemBuffer* RD_RenderingAPI_Vk::CreateVertexElemBuffer() {
	return new RD_RenderingAPI_VertexElemBufferVk();
}

RD_RenderingAPI_VertexBuffer* RD_RenderingAPI_Vk::CreateVertexBuffer() {
	return new RD_RenderingAPI_VertexBufferVk();
}

RD_RenderingAPI_VertexBufferInstanced* RD_RenderingAPI_Vk::CreateVertexBufferInstanced() {
	return new RD_RenderingAPI_VertexBufferInstancedVk();
}

RD_Texture* RD_RenderingAPI_Vk::CreateTexture() {
	return new RD_Texture_Vk();
}

RD_FrameBuffer* RD_RenderingAPI_Vk::CreateFrameBuffer(int w, int h, bool nodepth) {
	return new RD_FrameBuffer_Vk(w, h, nodepth);
}

RD_ShaderLoader* RD_RenderingAPI_Vk::CreateShader() {
	return new RD_ShaderLoader_Vk();
}

RD_Cubemap* RD_RenderingAPI_Vk::CreateCubemap() {
	return nullptr;
}

RD_UniformBuffer* RD_RenderingAPI_Vk::CreateUniformBuffer(const size_t buffersize, const int binding) {
	return new RD_UniformBuffer_Vk(buffersize, binding);
}

RD_ShaderStorageBuffer* RD_RenderingAPI_Vk::CreateShaderStorageBuffer(const size_t bufferSize, const int binding) {
	return new RD_ShaderStorageBuffer_Vk(bufferSize, binding);
}

void RD_RenderingAPI_Vk::SetViewportSize(int w, int h, int x, int y) {

}

void RD_RenderingAPI_Vk::Draw(RD_RenderingAPI_VertexElemBuffer* vbuff) {

}

void RD_RenderingAPI_Vk::DrawVB(RD_RenderingAPI_VertexBuffer* vbuff, DrawMode dm) {

}

void RD_RenderingAPI_Vk::DrawInstanced(RD_RenderingAPI_VertexBufferInstanced* vbuff, const int nbr, DrawMode dm) {

}

void RD_RenderingAPI_Vk::SetFilledMode(FillingMode fmode) {

}

void RD_RenderingAPI_Vk::Clear(int mask) {

}

void RD_RenderingAPI_Vk::SetClearColor(const vec3f& color) {

}

void RD_RenderingAPI_Vk::EnableFaceCulling() {

}

void RD_RenderingAPI_Vk::DisableFaceCulling() {

}

int RD_RenderingAPI_Vk::GetMaxTextureCount() {
	return 0;
}

bool RD_RenderingAPI_Vk::AreBindlessTexturesAvailable() {
	return true;
}

//------------------------------------------------------------

bool CheckDeviceExtensionSupport(VkPhysicalDevice dev) {
	uint32_t extCount = 0;
	vkEnumerateDeviceExtensionProperties(dev, nullptr, &extCount, nullptr);

	std::vector<VkExtensionProperties> availExt(extCount);
	vkEnumerateDeviceExtensionProperties(dev, nullptr, &extCount, availExt.data());

	std::set<std::string> reqExt(requestedExtensions.begin(), requestedExtensions.end());
	for (auto& ext : availExt) {
		reqExt.erase(ext.extensionName);
	}

	return reqExt.empty();
}