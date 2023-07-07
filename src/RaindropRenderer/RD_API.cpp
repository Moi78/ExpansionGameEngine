#include "RD_API.h"

#ifdef BUILD_VULKAN

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

//-----------------------------------------

RD_VulkanWindow::RD_VulkanWindow(std::shared_ptr<RD_API> api) {
    m_api = api;
    m_resizedFlag = false;
    m_vpm = RD_ViewportMode::FULL_WINDOW;
    m_vp = {0, 0, 1, 1};
    m_vp_real = {0, 0, 1, 1};

    m_initialized = false;
}

RD_VulkanWindow::~RD_VulkanWindow() {

}

void RD_VulkanWindow::CleanupVk(VkInstance inst, VkDevice dev, bool surfaceNoDelete) {
    if(!surfaceNoDelete) {
        m_vp_u.reset();
        m_screen_size.reset();

        m_rpass.reset();
        m_pline.reset();
        m_verticies.reset();

        if(m_overlay_sync.has_value()) {
            m_overlayed_rpass.value().reset();
            m_overlay_shader.value().reset();
            m_overlay_sync.value().reset();
            m_overlay_background.value().reset();
        }
    }

	if(!surfaceNoDelete) {
		vkDestroySemaphore(dev, m_imgAvail_s, nullptr);
		vkDestroySemaphore(dev, m_rndrFinished_s, nullptr);
		vkDestroyFence(dev, m_inFLight_f, nullptr);
	}

	for(auto fb : m_scFbs) {
		vkDestroyFramebuffer(dev, fb, nullptr);
	}

	for(auto view : m_scViews) {
		vkDestroyImageView(dev, view, nullptr);
	}

	vkDestroySwapchainKHR(dev, m_swapChain, nullptr);

	if(!surfaceNoDelete) {
		vkDestroySurfaceKHR(inst, m_surface, nullptr);
	}
}

RD_SwapChainDetails RD_VulkanWindow::QuerySwapchainSupport(VkPhysicalDevice dev) {
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

VkSurfaceFormatKHR RD_VulkanWindow::ChooseSwapFormat(const std::vector<VkSurfaceFormatKHR> availFmt) {
	for (const auto& fmt : availFmt) {
		if (fmt.format == VK_FORMAT_R8G8B8_SRGB && fmt.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
			return fmt;
		}
	}

	return availFmt[0]; // If the format that I want is not listed...
}

VkPresentModeKHR RD_VulkanWindow::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availPmodes) {
	for (auto& pmode : availPmodes) {
		if (pmode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return pmode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D RD_VulkanWindow::ChooseSwapExtent(VkSurfaceCapabilitiesKHR& cap) {
	if (cap.currentExtent.width != std::numeric_limits<uint32_t>::max() && cap.currentExtent.height != std::numeric_limits<uint32_t>::max()) {
		return cap.currentExtent;
	}
	else {
		int w, h;
		h = GetHeight();
		w = GetWidth();

		VkExtent2D ext = {
			static_cast<uint32_t>(w),
			static_cast<uint32_t>(h)
		};

		ext.height = std::clamp(ext.height, cap.minImageExtent.height, cap.maxImageExtent.height);
		ext.width = std::clamp(ext.width, cap.minImageExtent.width, cap.maxImageExtent.width);
		return ext;
	}
}

bool RD_VulkanWindow::CreateSwapChain(VkDevice dev, VkPhysicalDevice pdev, QueueFamilyIndices ind) {
	m_ind = ind;
	m_pdev = pdev;

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

		vkGetSwapchainImagesKHR(dev, m_swapChain, &imgCount, nullptr);
		m_scImages.resize(imgCount);
		vkGetSwapchainImagesKHR(dev, m_swapChain, &imgCount, m_scImages.data());

		m_scFormat = sfFormat.format;
		m_scExtent = extent;

		return CreateImageViews(dev);
	}
	else {
		return false;
	}
}

bool RD_VulkanWindow::CreateImageViews(VkDevice dev) {
	m_scViews.resize(m_scImages.size());

	for(int i = 0; i < m_scImages.size(); i++) {
		VkImageViewCreateInfo cInfo{};
		cInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		cInfo.image = m_scImages[i];
		cInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		cInfo.format = m_scFormat;
		cInfo.components = {
 			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY
		};
		cInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		cInfo.subresourceRange.baseMipLevel = 0;
		cInfo.subresourceRange.levelCount = 1;
		cInfo.subresourceRange.baseArrayLayer = 0;
		cInfo.subresourceRange.layerCount = 1;

		if(vkCreateImageView(dev, &cInfo, nullptr, &m_scViews[i]) != VK_SUCCESS) {
			std::cerr << "Failed to create ImageViews" << std::endl;
			return false;
		}
	}

	return true;
}


int RD_VulkanWindow::GetHeight() {
    if(!m_initialized) {
        return 0;
    }

    if(m_vpm == RD_ViewportMode::FULL_WINDOW) {
        return GetScreenHeight();
    } else {
        return m_vp.h;
    }
}

int RD_VulkanWindow::GetWidth() {
    if(!m_initialized) {
        return 0;
    }

    if(m_vpm == RD_ViewportMode::FULL_WINDOW) {
        return GetScreenWidth();
    } else {
        return m_vp.w;
    }
}

VkSurfaceKHR RD_VulkanWindow::GetSurfaceHandle() {
	return m_surface;
}

bool RD_VulkanWindow::MakeFramebuffers (VkDevice dev) {
	m_dev = dev; // Needed for the renderer to work, I know, this is ugly af

	m_scFbs.resize(m_scViews.size());

	std::shared_ptr<RD_RenderPass_Vk> rpassVK = std::reinterpret_pointer_cast<RD_RenderPass_Vk>(m_rpass);

	for(int i = 0; i < m_scViews.size(); i++) {
		VkFramebufferCreateInfo cInfo{};
		cInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		cInfo.renderPass = rpassVK->GetRenderPassHandle();
		cInfo.attachmentCount = 1;
		cInfo.pAttachments = &m_scViews[i];
		cInfo.width = m_scExtent.width;
		cInfo.height = m_scExtent.height;
		cInfo.layers = 1;

		if(vkCreateFramebuffer(dev, &cInfo, nullptr, &m_scFbs[i]) != VK_SUCCESS) {
			std::cerr << "Failed to create one of the swapchain's framebuffers." << std::endl;
			return false;
		}
	}

	return true;
}

void RD_VulkanWindow::Present() {
	vkWaitForFences(m_dev, 1, &m_inFLight_f, VK_TRUE, UINT64_MAX);

	if(vkAcquireNextImageKHR(m_dev, m_swapChain, UINT64_MAX, m_imgAvail_s, VK_NULL_HANDLE, &m_imgIdx) == VK_ERROR_OUT_OF_DATE_KHR) {
		std::cout << "OUT OF DATE" << std::endl;
		ResizeFrame(GetWidth(), GetHeight());
		//vkResetFences(m_dev, 1, &m_inFLight_f);

		return;
	}

    uint32_t isFS = false;

	std::shared_ptr<RD_Pipeline_Vk> plineVK = std::reinterpret_pointer_cast<RD_Pipeline_Vk>(m_pline);

    auto sync = m_overlay_sync.value();
    sync->Start();
    m_overlayed_rpass.value()->BeginRenderpass(sync);

    if(m_overlayed_rpass.has_value()) {
        m_overlay_background.value()->Bind(sync);
        m_overlay_background.value()->PushConstant(&isFS, sizeof(uint32_t), sync);

        m_overlay_background.value()->DrawIndexedVertexBuffer(m_verticies, sync);

        m_overlay_background.value()->Unbind(sync);

        isFS = true;

        m_overlay_shader.value()->Bind(sync);
        m_overlay_shader.value()->PushConstant(&isFS, sizeof(uint32_t), sync);

        m_overlay_shader.value()->DrawIndexedVertexBuffer(m_verticies, sync);

        m_overlay_shader.value()->Unbind(sync);
    }

    isFS = true;

    m_imgui_pline->Bind(sync);
    m_imgui_pline->PushConstant(&isFS, sizeof(uint32_t), sync);
    m_imgui_pline->DrawIndexedVertexBuffer(m_verticies, sync);
    m_imgui_pline->Unbind(sync);

    m_overlayed_rpass.value()->EndRenderpass(sync);
    sync->Stop();

    plineVK->BindSC(m_scFbs[m_imgIdx]);
    plineVK->PushConstant(&isFS, sizeof(uint32_t), {});

    plineVK->DrawIndexedVertexBuffer(m_verticies, {});

	plineVK->UnbindSC();

	vkResetFences(m_dev, 1, &m_inFLight_f);

	plineVK->SubmitCMD(m_gfxQueue, m_imgAvail_s, m_rndrFinished_s, m_inFLight_f);

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &m_rndrFinished_s;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &m_swapChain;
	presentInfo.pImageIndices = &m_imgIdx;
	presentInfo.pResults = nullptr;

	vkQueuePresentKHR(m_presentQueue, &presentInfo);
}

bool RD_VulkanWindow::BuildSyncObjects(VkDevice dev) {
	VkSemaphoreCreateInfo scInfo{};
	scInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fcInfo{};
	fcInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fcInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if(vkCreateSemaphore(dev, &scInfo, nullptr, &m_imgAvail_s) != VK_SUCCESS ||
	   vkCreateSemaphore(dev, &scInfo, nullptr, &m_rndrFinished_s) != VK_SUCCESS ||
	   vkCreateFence(dev, &fcInfo, nullptr, &m_inFLight_f) != VK_SUCCESS
	) {
		std::cerr << "Failed to create window sync objects." << std::endl;
		return false;
	}

	return true;
}

void RD_VulkanWindow::SendGraphicsQueue(VkQueue queue) {
	m_gfxQueue = queue;
}

void RD_VulkanWindow::SendPresentQueue(VkQueue queue) {
	m_presentQueue = queue;
}

bool RD_VulkanWindow::ResizeFrame(const int w, const int h) {
	vkDeviceWaitIdle(m_dev);
	m_resizedFlag = true;

	CleanupVk(m_inst, m_dev, true);

	bool res_a = CreateSwapChain(m_dev, m_pdev, m_ind);
	bool res_b = MakeFramebuffers(m_dev);

    m_rpass->SetRenderpassSize(nullptr, m_scExtent.width, m_scExtent.height);
    m_pline->RebuildPipeline();

    RD_Rect currentSize = {
            .x = 0.0f,
            .y = 0.0f,
            .w = (float)GetScreenWidth(),
            .h = (float)GetScreenHeight()
    };

    m_screen_size->FillBufferData(&currentSize);

    if(m_overlayed_rpass.has_value()) {
        m_overlayed_rpass.value()->SetRenderpassSize(m_api.get(), m_scExtent.width, m_scExtent.height);
    }

    if(m_overlay_shader.has_value()) {
        m_overlay_shader.value()->RebuildPipeline();
        m_overlay_background.value()->RebuildPipeline();
    }

    m_imgui_rpass->SetRenderpassSize(m_api.get(), m_scExtent.width, m_scExtent.height);

    m_imgui_ui = m_imgui_rpass->GetAttachment(0);

    m_imgui_pline->PurgeTextures();
    m_imgui_pline->RegisterTexture(m_imgui_ui, 0);
    m_imgui_pline->RebuildPipeline();

    if(m_vpm == RD_ViewportMode::FLOATING) {
        vec2 offset = vec2(m_vp.w / (float)GetScreenWidth(), (m_vp.h / (float)GetScreenHeight()));
        offset = offset + (vec2(m_vp.x / (float)GetScreenWidth(), (m_vp.y / (float)GetScreenHeight())) * 2);
        offset = offset - vec2(1, 1);

        m_vp_real = {
                offset.GetX(), offset.GetY(),
                m_vp.w / (float)GetScreenWidth(),
                m_vp.h / (float)GetScreenHeight()
        };
        m_vp_u->FillBufferData(&m_vp_real);
    }

    if(m_extCallback.has_value()) {
        m_extCallback->get()->Call();
    }

	return res_a && res_b;
}

void RD_VulkanWindow::SetExternalResizeCallback(std::shared_ptr<RD_Callback> cbck) {
    m_extCallback = cbck;
}

bool RD_VulkanWindow::WasResized() {
	bool buff = m_resizedFlag;
	m_resizedFlag = false;

	return buff;
}

void RD_VulkanWindow::BuildBlitPipeline(std::string enginePath) {
    std::shared_ptr<RD_ShaderLoader> blitShader = m_api->CreateShader();
    blitShader->CompileShaderFromFile(enginePath + "/shaders/bin/sc_blit.vspv", enginePath + "/shaders/bin/sc_blit.fspv");

    const int scrW = GetScreenWidth();
    const int scrH = GetScreenHeight();

    RD_Attachment att{};
    att.do_clear = true;
    att.format = IMGFORMAT_BGRA;
    att.is_swapchain_attachment = true;
    att.sample_count = 1;

    m_rpass = m_api->CreateRenderPass({att}, scrW, scrH);
    m_rpass->BuildRenderpass(m_api.get(), true);

    m_pline = m_api->CreatePipeline(m_rpass, blitShader, true);

    m_imgui_pline = m_api->CreatePipeline(m_imgui_rpass, blitShader);

    m_vp_u = m_api->CreateUniformBuffer(80);
    m_vp_u->BuildAndAllocateBuffer(sizeof(RD_Rect));
    m_vp_u->FillBufferData(&m_vp);

    m_screen_size = m_api->CreateUniformBuffer(81);
    m_screen_size->BuildAndAllocateBuffer(sizeof(RD_Rect));

    const RD_Rect currentSize = {
            .x = 0.0f,
            .y = 0.0f,
            .w = (float)scrW,
            .h = (float)scrH
    };

    const mat4f scr_mat = ProjOrtho(0, scrW, 0, scrH, -1, 1);

    m_screen_size->PartialFillBufferData((void*)&currentSize, 4 * sizeof(float), 0);
    m_screen_size->PartialFillBufferData((void*)&currentSize, 4 * sizeof(float), 0);

    m_pline->RegisterUniformBuffer(m_vp_u);
    m_pline->ConfigurePushConstant(sizeof(uint32_t));
    m_pline->BuildPipeline();

    m_verticies = m_api->CreateIndexedVertexBuffer();

    auto vData = MakeVertexData(
            { vec3(-1.0, 1.0, 0.0), vec3(-1.0, -1.0, 0.0), vec3(1.0, 1.0, 0.0), vec3(1.0, -1.0, 0.0) },
            { vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0) },
            { vec2(0.0, 1.0), vec2(0.0, 0.0), vec2(1.0, 1.0), vec2(1.0, 0.0)}
    );

    std::vector<uint32_t> indices = {
            0, 1, 2,
            1, 3, 2
    };

    m_verticies->FillBufferData(vData, indices);
}

void RD_VulkanWindow::SetPresentTexture(std::shared_ptr<RD_Texture> tex) {
    m_presentTex = tex;

    m_pline->PurgeTextures();
    m_pline->RegisterTexture(tex, 0);
    m_pline->RebuildPipeline();
}

void RD_VulkanWindow::SetViewportMode(RD_ViewportMode vpm) {
    m_vpm = vpm;

    if(m_vpm == RD_ViewportMode::FLOATING) {
        ResizeFrame(m_vp.w, m_vp.h);
    } else if(m_vpm == RD_ViewportMode::FULL_WINDOW) {
        RD_Rect defRect = {0, 0, 1, 1};
        m_vp_u->FillBufferData(&defRect);
    }
}

void RD_VulkanWindow::SetViewport(RD_Rect vp) {
    bool sizeChanged = (vp.w != m_vp.w) || (vp.h != m_vp.h);
    m_vp = vp;

    if(m_vpm == RD_ViewportMode::FLOATING && sizeChanged) {
        ResizeFrame(vp.w, vp.h);
    } else {
        UpdateViewport();
    }
}

void RD_VulkanWindow::UpdateViewport() {
    if(m_vpm == RD_ViewportMode::FLOATING) {
        vec2 offset = vec2(m_vp.w / (float)GetScreenWidth(), (m_vp.h / (float)GetScreenHeight()));
        offset = offset + (vec2(m_vp.x / (float)GetScreenWidth(), (m_vp.y / (float)GetScreenHeight())) * 2);
        offset = offset - vec2(1, 1);

        m_vp_real = {
                offset.GetX(), offset.GetY(),
                m_vp.w / (float)GetScreenWidth(),
                m_vp.h / (float)GetScreenHeight()
        };
        m_vp_u->FillBufferData(&m_vp_real);
    }
}

RD_Rect RD_VulkanWindow::GetViewportRect() {
    return m_vp_real;
}

bool RD_VulkanWindow::HasPresentTexture() {
    return !m_presentTex.expired();
}

std::shared_ptr<RD_Texture> RD_VulkanWindow::GetPresentTexture() {
    return m_presentTex.lock();
}

void RD_VulkanWindow::EnableOverlaying(std::shared_ptr<RD_Texture> overlay, std::string enginePath) {
    RD_Attachment color {
        .format = IMGFORMAT_RGBA,
        .sample_count = 1,
        .do_clear = true,
        .clearColor = vec4(0.0f, 0.0f, 0.0f, 1.0f),
        .is_swapchain_attachment = false,
        .is_transparent = true,
    };

    if(!m_overlayed_rpass.has_value()) {
        m_overlayed_rpass = m_api->CreateRenderPass({color}, GetScreenWidth(), GetScreenHeight());
        m_overlayed_rpass.value()->BuildRenderpass(m_api.get(), false);
    }

    auto sh_loader = m_api->CreateShader();
    sh_loader->CompileShaderFromFile(enginePath + "/shaders/bin/sc_blit.vspv", enginePath + "/shaders/bin/sc_blit.fspv");

    m_overlay_shader = m_api->CreatePipeline(m_overlayed_rpass.value(), sh_loader);
    m_overlay_shader.value()->RegisterTexture(overlay, 0);
    m_overlay_shader.value()->ConfigurePushConstant(sizeof(uint32_t));
    m_overlay_shader.value()->RegisterUniformBuffer(m_vp_u);
    m_overlay_shader.value()->BuildPipeline();

    m_overlay_background = m_api->CreatePipeline(m_overlayed_rpass.value(), sh_loader);
    m_overlay_background.value()->RegisterTexture(GetPresentTexture(), 0);
    m_overlay_background.value()->ConfigurePushConstant(sizeof(uint32_t));
    m_overlay_background.value()->RegisterUniformBuffer(m_vp_u);
    m_overlay_background.value()->BuildPipeline();

    m_overlay_sync = m_api->CreateRenderSynchronizer();

    SetPresentTexture(m_overlayed_rpass.value()->GetAttachment(0));
}

void RD_VulkanWindow::SetOverlayTexture(std::shared_ptr<RD_Texture> overlay) {
    m_overlay_shader.value()->PurgeTextures();
    m_overlay_shader.value()->RegisterTexture(overlay, 0);
    m_overlay_shader.value()->RebuildPipeline();
}

void RD_VulkanWindow::UpdateOverlaying() {
    auto bg = m_overlay_background.value();
    bg->PurgeTextures();
    bg->RegisterTexture(GetPresentTexture(), 0);
    bg->RebuildPipeline();

    SetPresentTexture(m_overlayed_rpass.value()->GetAttachment(0));
}

std::shared_ptr<RD_UniformBuffer> RD_VulkanWindow::GetScreenSizeBuffer() {
    return m_screen_size;
}

// ------------------------------------------------------------------------------------

RD_Windowing_GLFW_Vk::RD_Windowing_GLFW_Vk(std::shared_ptr<RD_API> api) : RD_VulkanWindow(api) {
    m_curHidden = false;

    m_w = 0;
    m_h = 0;

    m_resizedFlag = false;
    m_initialized = false;
}

RD_Windowing_GLFW_Vk::~RD_Windowing_GLFW_Vk() {
    ImGui::DestroyContext();
    ImNodes::DestroyContext();

    glfwDestroyWindow(m_win);
    glfwTerminate();
}

void RD_Windowing_GLFW_Vk::ResizeCBCK(GLFWwindow* win, int w, int h) {
    auto app = reinterpret_cast<RD_VulkanWindow*>(glfwGetWindowUserPointer(win));
    app->ResizeFrame(w, h);
}

bool RD_Windowing_GLFW_Vk::OpenWindow(std::string name, const int w, const int h) {
    if (!glfwInit()) {
        std::cerr << "ERROR: Failed to init GLFW." << std::endl;
        return false;
    }

    m_w = w;
    m_h = h;

    m_resizedFlag = false;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    m_win = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_win, this);
    glfwSetFramebufferSizeCallback(m_win, RD_Windowing_GLFW_Vk::ResizeCBCK);

    if (m_win == nullptr) {
        std::cerr << "ERROR: Cannot create the GLFW window." << std::endl;
        return false;
    }

    m_initialized = true;

    return true;
}

bool RD_Windowing_GLFW_Vk::WantToClose() {
    return glfwWindowShouldClose(m_win) == GLFW_TRUE;
}

int RD_Windowing_GLFW_Vk::GetScreenHeight() {
    glfwGetFramebufferSize(m_win, NULL, &m_h);
    return m_h;
}

int RD_Windowing_GLFW_Vk::GetScreenWidth() {
    glfwGetFramebufferSize(m_win, &m_w, nullptr);
    return m_w;
}

GLFWwindow* RD_Windowing_GLFW_Vk::GetWindow() {
    return m_win;
}

void RD_Windowing_GLFW_Vk::PollEvents() {
    glfwPollEvents();
}

float RD_Windowing_GLFW_Vk::GetCursorPositionX(bool abs) {
    if(!abs && !m_curHidden) {
        return 0;
    } else if(abs && m_curHidden) {
        return 0;
    }

    double x = 0.0f;
    glfwGetCursorPos(m_win, &x, nullptr);

    return (float)x;
}

float RD_Windowing_GLFW_Vk::GetCursorPositionY(bool abs) {
    if(!abs && !m_curHidden) {
        return 0;
    } else if(abs && m_curHidden) {
        return 0;
    }

    double y = 0.0f;
    glfwGetCursorPos(m_win, nullptr, &y);

    return (float)y;
}

void RD_Windowing_GLFW_Vk::SetCursorVisibility(bool visibility) {
    m_curHidden = !visibility;

    glfwSetInputMode(m_win, GLFW_CURSOR, visibility ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

void RD_Windowing_GLFW_Vk::SetCursorPosition(double x, double y) {
    glfwSetCursorPos(m_win, x, y);
}

bool RD_Windowing_GLFW_Vk::GetKeyPress(int key) {
    return glfwGetKey(m_win, key) == GLFW_PRESS;
}

bool RD_Windowing_GLFW_Vk::GetMouseButtonPress(int mbutton) {
    return glfwGetMouseButton(m_win, mbutton) == GLFW_PRESS;
}

int RD_Windowing_GLFW_Vk::GetExtensionsCount() {
    uint32_t count = 0;
    glfwGetRequiredInstanceExtensions(&count);

    return count;
}

const char** RD_Windowing_GLFW_Vk::GetExtensionsNames() {
    uint32_t count;
    return glfwGetRequiredInstanceExtensions(&count);
}

VkResult RD_Windowing_GLFW_Vk::CreateWindowSurface(VkInstance inst) {
    assert(m_win != nullptr && "A window must be opened to create a surface");
    m_inst = inst;

    VkResult res = glfwCreateWindowSurface(inst, m_win, nullptr, &m_surface);

    return res;
}

void chk_vk_res(VkResult res) {
    assert(res == VK_SUCCESS && "ImGui failure");
}

void RD_Windowing_GLFW_Vk::InitImgui(std::string enginePath) {
    RD_Attachment color = {};
    color.do_clear = true;
    color.format = IMGFORMAT_RGBA;
    color.is_swapchain_attachment = false;
    color.sample_count = 1;
    color.clearColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    color.is_transparent = true;

    auto rpass = m_api->CreateRenderPass({color}, GetScreenWidth(), GetScreenHeight());
    if(!rpass->BuildRenderpass(m_api.get(), false)) {
        std::cerr << "FAILED TO INIT IMGUI (RPASS BUILD FAILURE)" << std::endl;
        return;
    }
    m_imgui_rpass = std::reinterpret_pointer_cast<RD_RenderPass_Vk>(rpass);

    auto rsync = m_api->CreateRenderSynchronizer();
    m_imgui_rsync = std::reinterpret_pointer_cast<RD_RenderSynchronizer_Vk>(rsync);

    VkDescriptorPoolSize psizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };

    VkDescriptorPoolCreateInfo pcInfo{};
    pcInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pcInfo.maxSets = 1000 * IM_ARRAYSIZE(psizes);
    pcInfo.pPoolSizes = psizes;
    pcInfo.poolSizeCount = (uint32_t) IM_ARRAYSIZE(psizes);

    VkResult res = vkCreateDescriptorPool(m_dev, &pcInfo, nullptr, &m_imgui_pool);
    if(res != VK_SUCCESS) {
        std::cerr << "FAILED TO INIT IMGUI (DESC POOL FAIL)" << std::endl;
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.Fonts->AddFontFromFileTTF(std::string(enginePath + "/ui/fonts/opensans.ttf").c_str(), 15.0f);

    ImGui_ImplGlfw_InitForVulkan(m_win, true);

    ImGui_ImplVulkan_InitInfo i_inf{};
    i_inf.Instance = m_inst;
    i_inf.PhysicalDevice = m_pdev;
    i_inf.Device = m_dev;
    i_inf.QueueFamily = m_ind.graphicsFamily.value();
    i_inf.Queue = m_gfxQueue;
    i_inf.PipelineCache = VK_NULL_HANDLE;

    i_inf.DescriptorPool = m_imgui_pool;

    i_inf.Subpass = 0;
    i_inf.MinImageCount = 2;
    i_inf.ImageCount = 3;
    i_inf.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    i_inf.Allocator = nullptr;
    i_inf.CheckVkResultFn = chk_vk_res;

    ImGui_ImplVulkan_Init(&i_inf, m_imgui_rpass->GetRenderPassHandle());

    auto vkapi = std::reinterpret_pointer_cast<RD_API_Vk>(m_api);
    auto cmd = BeginOneTimeCommand(m_dev, vkapi->GetCommandPoolHdl());

    ImGui_ImplVulkan_CreateFontsTexture(cmd);

    EndOneTimeCommand(m_dev, vkapi->GetCommandPoolHdl(), cmd, m_gfxQueue);

    ImGui_ImplVulkan_DestroyFontUploadObjects();

    m_imgui_ui = m_imgui_rpass->GetAttachment(0);

    m_overlayed_rpass = m_api->CreateRenderPass({color}, GetScreenWidth(), GetScreenHeight());
    m_overlayed_rpass.value()->BuildRenderpass(m_api.get(), false);

    std::shared_ptr<RD_ShaderLoader> blitShader = m_api->CreateShader();
    blitShader->CompileShaderFromFile(enginePath + "/shaders/bin/sc_blit.vspv", enginePath + "/shaders/bin/sc_blit.fspv");

    m_imgui_pline = m_api->CreatePipeline(m_imgui_rpass, blitShader);
    m_imgui_pline->ConfigurePushConstant(sizeof(uint32_t));
    m_imgui_pline->RegisterTexture(m_imgui_ui, 0);
    m_imgui_pline->RegisterUniformBuffer(m_vp_u);
    m_imgui_pline->BuildPipeline();

    ImNodes::CreateContext();
}

void RD_Windowing_GLFW_Vk::ImguiNewFrame() {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void RD_Windowing_GLFW_Vk::ImguiEndFrame() {
    ImGui::Render();

    //ImGui::UpdatePlatformWindows();
    //ImGui::RenderPlatformWindowsDefault();

    ImDrawData* data = ImGui::GetDrawData();

    m_imgui_rsync->Start();
    m_imgui_rpass->BeginRenderpass(m_imgui_rsync);

    ImGui_ImplVulkan_RenderDrawData(data, m_imgui_rsync->GetCommandBuffer());

    m_imgui_rpass->EndRenderpass(m_imgui_rsync);
    m_imgui_rsync->Stop();
}

// ------------------------------------------------------------------------------------

RD_API_Vk::RD_API_Vk() {
	m_validationLayers = true;

    m_pool = VK_NULL_HANDLE;
    m_ldev = VK_NULL_HANDLE;
    m_dev = VK_NULL_HANDLE;
    m_inst = VK_NULL_HANDLE;
    m_gfx_queue = VK_NULL_HANDLE;
    m_present_queue = VK_NULL_HANDLE;
    m_cbck_dbg = VK_NULL_HANDLE;
}

RD_API_Vk::~RD_API_Vk() {
	vkDestroyCommandPool(m_ldev, m_pool, nullptr);

	m_winsys->CleanupVk(m_inst, m_ldev);

	vkDestroyDevice(m_ldev, nullptr);
	if (m_validationLayers) {
		vkDestroyDebugUtilsMessengerEXT(m_inst, m_cbck_dbg, nullptr);
	}

	vkDestroyInstance(m_inst, nullptr);
}

void RD_API_Vk::SetWindowSystem(std::shared_ptr<RD_Windowing> win) {
    m_winsys = std::reinterpret_pointer_cast<RD_VulkanWindow>(win);
}

bool RD_API_Vk::InitAPI(std::string name, const int w, const int h) {
	m_winsys->OpenWindow(name, w, h);

	if (!CreateVkInst()) {
		return false;
	}

	if (m_validationLayers) {
		if (!SetupDebugMessenger()) {
			return false;
		}
	}

	if (m_winsys->CreateWindowSurface(m_inst) != VK_SUCCESS) {
		std::cerr << "Failed to create window surface." << std::endl;
		return false;
	}

	PickPhysicalDevice();

	if (!CreateDevice()) {
		return false;
	}

	QueueFamilyIndices qFamInd = FindQueueFamilies(m_dev);
	if (!m_winsys->CreateSwapChain(m_ldev, m_dev, qFamInd)) {
		std::cerr << "Failed to create swapchain." << std::endl;;
		return false;
	}

	m_winsys->BuildSyncObjects(m_ldev);
	m_winsys->SendGraphicsQueue(m_gfx_queue);
	m_winsys->SendPresentQueue(m_present_queue);

	if(!CreateCommandPool()) {
		return false;
	}

	return true;
}

std::shared_ptr<RD_ShaderLoader> RD_API_Vk::CreateShader() {
	return std::make_shared<RD_ShaderLoader_Vk>(m_ldev);
}

std::shared_ptr<RD_RenderPass> RD_API_Vk::CreateRenderPass ( std::vector<RD_Attachment> attachments, float width, float height ) {
	return std::make_shared<RD_RenderPass_Vk>(m_ldev, attachments, width, height);
}

std::shared_ptr<RD_Pipeline> RD_API_Vk::CreatePipeline ( std::shared_ptr<RD_RenderPass> rpass, std::shared_ptr<RD_ShaderLoader> shader, bool extSignaling) {
	return std::make_shared<RD_Pipeline_Vk>(m_ldev, m_pool, m_gfx_queue, rpass, shader, extSignaling);
}

std::shared_ptr<RD_Windowing> RD_API_Vk::GetWindowingSystem() {
	return m_winsys;
}


bool RD_API_Vk::CreateVkInst() {
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
		std::cerr << "Failed to create VkInstance." << std::endl;
		return false;
	}

	return true;
}

bool RD_API_Vk::CheckRequestedValidationLayers(std::vector<std::string> layers) {
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> layerProp(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, layerProp.data());

	for (auto layer : layers) {
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

bool RD_API_Vk::SetupDebugMessenger() {
	VkDebugUtilsMessengerCreateInfoEXT cInfo{};
	cInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	cInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
							VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
							VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

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

void RD_API_Vk::PickPhysicalDevice() {
	m_dev = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_inst, &deviceCount, nullptr);
	if (deviceCount == 0) {
		std::cerr << "No Vulkan capable GPU found." << std::endl;
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_inst, &deviceCount, devices.data());

	if (deviceCount == 1) {
		m_dev = devices[0];

		VkPhysicalDeviceProperties prop;
		vkGetPhysicalDeviceProperties(m_dev, &prop);
		std::cout << "Usign GPU : " << prop.deviceName << std::endl;

		return;
	}

	for (auto dev : devices) {
		if (IsDeviceSuitable(dev)) {
			m_dev = dev;
			break;
		}
	}

    if(m_dev == VK_NULL_HANDLE) {
        std::cout << "No suitable devices..." << std::endl;
        exit(-1);
    }

	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(m_dev, &prop);
	std::cout << "Using GPU : " << prop.deviceName << std::endl;
}

bool RD_API_Vk::CreateDevice() {
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
    features.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo devcInfo{};
	devcInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	devcInfo.pQueueCreateInfos = devCInfo.data();
	devcInfo.queueCreateInfoCount = static_cast<uint32_t>(devCInfo.size());
	devcInfo.pEnabledFeatures = &features;
	devcInfo.enabledExtensionCount = static_cast<uint32_t>(requestedExtensions.size());
	devcInfo.ppEnabledExtensionNames = requestedExtensions.data();

	VkResult res = vkCreateDevice(m_dev, &devcInfo, nullptr, &m_ldev);
	if (res != VK_SUCCESS) {
		std::cerr << "ERROR: Failed to create a logical device." << std::endl;
		return false;
	}

	vkGetDeviceQueue(m_ldev, ind.graphicsFamily.value(), 0, &m_gfx_queue);
	vkGetDeviceQueue(m_ldev, ind.presentationFamily.value(), 0, &m_present_queue);

	return true;
}

QueueFamilyIndices RD_API_Vk::FindQueueFamilies(VkPhysicalDevice dev) {
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
		vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, m_winsys->GetSurfaceHandle(), &presSupport);
		if (presSupport) {
			indices.presentationFamily = i;
		}

		i++;
	}

	return indices;
}

std::vector<const char*> RD_API_Vk::GetRequiredExtensions() {
	uint32_t extCount;
	const char** extNames;
	extNames = glfwGetRequiredInstanceExtensions(&extCount);

	std::vector<const char*> requiredExtensions(extNames, extNames + extCount);
	if (m_validationLayers) {
		requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return requiredExtensions;
}

bool RD_API_Vk::IsDeviceSuitable(VkPhysicalDevice dev) {
	VkPhysicalDeviceProperties prop;
	vkGetPhysicalDeviceProperties(dev, &prop);

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(dev, &features);

	QueueFamilyIndices qFam = FindQueueFamilies(dev);

	RD_SwapChainDetails scDet{};
	scDet = m_winsys->QuerySwapchainSupport(dev);

	bool isSwapchainSuitable = !(scDet.fmt.empty()) && !(scDet.pmode.empty());

	return (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && qFam.IsComplete(dev) && isSwapchainSuitable;
}

bool RD_API_Vk::MakeFramebuffers () {
	return m_winsys->MakeFramebuffers(m_ldev);
}

bool RD_API_Vk::CreateCommandPool() {
	QueueFamilyIndices qfInd = FindQueueFamilies(m_dev);

	VkCommandPoolCreateInfo cInfo{};
	cInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	cInfo.queueFamilyIndex = qfInd.graphicsFamily.value();

	if(vkCreateCommandPool(m_ldev, &cInfo, nullptr, &m_pool) != VK_SUCCESS) {
		std::cerr << "Failed to create command pool." << std::endl;
		return false;
	}

	return true;
}

void RD_API_Vk::ProperQuit() {
	vkDeviceWaitIdle(m_ldev);
}

std::shared_ptr<RD_VertexBuffer> RD_API_Vk::CreateVertexBuffer() {
    return std::make_shared<RD_VertexBuffer_Vk>(m_ldev, m_dev, m_gfx_queue, m_pool);
}

std::shared_ptr<RD_IndexedVertexBuffer> RD_API_Vk::CreateIndexedVertexBuffer() {
    return std::make_shared<RD_IndexedVertexBuffer_Vk>(m_ldev, m_dev, m_gfx_queue, m_pool);
}

std::shared_ptr<RD_UniformBuffer> RD_API_Vk::CreateUniformBuffer(uint32_t binding) {
    return std::make_shared<RD_UniformBuffer_Vk>(m_ldev, m_dev, binding);
}

std::shared_ptr<RD_Texture> RD_API_Vk::CreateTexture() {
    return std::make_shared<RD_Texture_Vk>(m_ldev, m_dev, m_gfx_queue, m_pool);
}

std::shared_ptr<RD_RenderSynchronizer> RD_API_Vk::CreateRenderSynchronizer() {
    return std::make_shared<RD_RenderSynchronizer_Vk>(m_ldev, m_pool, m_gfx_queue);
}

std::shared_ptr<RD_OrphanFramebuffer> RD_API_Vk::CreateOrphanFramebuffer(
        std::shared_ptr<RD_RenderPass> rpass,
        std::vector<RD_Attachment> attachments,
        float w, float h
) {
    return std::make_shared<RD_OrphanFramebuffer_Vk>(m_ldev, rpass, attachments, w, h);
}

//-----------------------------------

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

#endif
