#include "pch.h"
#include "RD_RenderingAPI_Vk.h"

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
	glfwGetWindowSize(m_win, NULL, &m_h);
	return m_h;
}

int RD_WindowingSystemGLFW_Vk::GetWidth() {
	glfwGetWindowSize(m_win, &m_w, NULL);
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

	PickPhysicalDevice();

	if (!CreateDevice()) {
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

	VkDeviceQueueCreateInfo cInfo{};
	cInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	cInfo.queueCount = 1;
	cInfo.queueFamilyIndex = ind.graphicsFamily.value();

	float prio = 1.0f;
	cInfo.pQueuePriorities = &prio;

	VkPhysicalDeviceFeatures features{};

	VkDeviceCreateInfo devcInfo{};
	devcInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	devcInfo.pQueueCreateInfos = &cInfo;
	devcInfo.queueCreateInfoCount = 1;
	devcInfo.pEnabledFeatures = &features;

	VkResult res = vkCreateDevice(m_dev, &devcInfo, nullptr, &m_ldev);
	if (res != VK_SUCCESS) {
		dispErrorMessageBox(L"ERROR: Failed to create a logical device.");
		return false;
	}

	vkGetDeviceQueue(m_ldev, ind.graphicsFamily.value(), 0, &m_gfx_queue);

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

	return (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && (qFam.graphicsFamily.has_value());
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
	cInfo.enabledExtensionCount = extensions.size();
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
	return nullptr;
}

RD_FrameBuffer* RD_RenderingAPI_Vk::CreateFrameBuffer(int w, int h, bool nodepth) {
	return nullptr;
}

RD_ShaderLoader* RD_RenderingAPI_Vk::CreateShader() {
	return nullptr;
}

RD_Cubemap* RD_RenderingAPI_Vk::CreateCubemap() {
	return nullptr;
}

RD_UniformBuffer* RD_RenderingAPI_Vk::CreateUniformBuffer(const size_t buffersize, const int binding) {
	return nullptr;
}

RD_ShaderStorageBuffer* RD_RenderingAPI_Vk::CreateShaderStorageBuffer(const size_t bufferSize, const int binding) {
	return nullptr;
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