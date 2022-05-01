#include "pch.h"
#include "RD_RenderingPipelinePBR.h"

RD_RenderingPipelinePBR::RD_RenderingPipelinePBR() {

}

RD_RenderingPipelinePBR::~RD_RenderingPipelinePBR() {
	delete m_quad;

	delete m_gbuff;
	delete m_light_fb;
	delete m_shadow_blur_a;
	delete m_shadow_blur_b;

	delete m_light;
	delete m_beauty;
	delete m_shadow;
	delete m_shadowBlur;
	delete m_shadowCalc;
	delete m_ssao;
	delete m_ssaoBlur;
	delete m_reflections;
	delete m_debug;

	delete m_gbuff_handles;
	delete m_shadowmaps;
	delete m_lspace_mats;
	delete m_blurState;
	delete m_sfx_handles;
}

void RD_RenderingPipelinePBR::InitPipeline(RD_RenderingAPI* api, std::string engineDir) {
	InitUBO(api);
	CompileShaders(api, engineDir);
	CreateGBuff(api);

	m_quad = new RD_Quad(api);
	m_quad->Bufferize();
}

void RD_RenderingPipelinePBR::RenderScene(RD_RenderingAPI* api, RD_MaterialLibrary* sceneMeshData) {
	m_gbuff->BindFBO();
	api->Clear(COLOR_BUFFER | DEPTH_BUFFER);

	for (auto mat : sceneMeshData->GetRawVector()) {
		mat.second->DrawMeshes();
	}

	m_gbuff->UnbindFBO();
	if (m_gbuff->IsFBMultisampled()) {
		m_gbuff->MultisampledToIntermediate();
	}

	m_light_fb->BindFBO();
	api->Clear(COLOR_BUFFER);

	m_light->useShader();

	if (m_gbuff->GetAttachementByIndex(m_gbuff_struct.gAlbedo)->BindTexture(0)) m_light->SetInt("gAlbedo", 0);
	if (m_gbuff->GetAttachementByIndex(m_gbuff_struct.gNorm)->BindTexture(1)) m_light->SetInt("gNorm", 1);
	if (m_gbuff->GetAttachementByIndex(m_gbuff_struct.gPos)->BindTexture(2)) m_light->SetInt("gPos", 2);
	if (m_gbuff->GetAttachementByIndex(m_gbuff_struct.gSpec)->BindTexture(3)) m_light->SetInt("gSpec", 3);
	if (m_shadow_blur_b->GetAttachementByIndex(0)->BindTexture(4)) m_light->SetInt("shadowPass", 4); // SHADOW PASS
	if (m_gbuff->GetAttachementByIndex(m_gbuff_struct.gMetRoughAO)->BindTexture(5)) m_light->SetInt("gMetRoughAO", 5);
	if (m_gbuff->GetAttachementByIndex(m_gbuff_struct.gAlbedo)->BindTexture(6)) m_light->SetInt("", 6); // SSAO
	if (m_gbuff->GetAttachementByIndex(m_gbuff_struct.gEmissive)->BindTexture(7)) m_light->SetInt("gEmissive", 7);

	m_quad->RenderQuad();

	m_light_fb->UnbindFBO();

	m_beauty->useShader();
	m_quad->RenderQuad();
}

void RD_RenderingPipelinePBR::RenderShadowMaps(RD_RenderingAPI* api, std::vector<RD_DirLight*> dlights, RD_MaterialLibrary* sceneData, vec3f camPos) {
	m_shadow->useShader();

	int texID = 0, i = 0;
	for (auto dlight : dlights) {
		if (!dlight->GetShadowCasting()) {
			continue;
		}

		dlight->DepthRender(api, m_shadow, sceneData, camPos);

		if (dlight->GetDepthTexID()->BindTexture(texID)) {
			m_shadowCalc->SetInt("ShadowMap[" + std::to_string(i) + "]", texID);
		}

		texID++, i++;
	}

	m_lspace_mats->BindBuffer();
	int idx = 0;
	for (auto dlight : dlights) {
		m_lspace_mats->SetBufferSubData(idx * 64, 64, dlight->GetLightSpace().GetPTR());
	}
	m_lspace_mats->UnbindBuffer();

	if (m_gbuff->GetAttachementByIndex(m_gbuff_struct.gPos)->BindTexture(texID)) {
		m_shadowCalc->SetInt("gPos", texID);
	}

	m_shadowCalc->useShader();
	m_shadow_fb->BindFBO();

	m_quad->RenderQuad();

	m_shadow_fb->UnbindFBO();

	BlurShadow();
}

void RD_RenderingPipelinePBR::CompileShaders(RD_RenderingAPI* api, std::string engineDir) {
	std::cout << "Compiling PBR Pipeline shaders." << std::endl;

	API type = api->GetAPIType();

	std::string shaderPath;

	std::string shaderFragExt;
	std::string shaderVertExt;
	if (type == API::OPENGL3) {
		shaderPath = engineDir + "/shaders/glsl/gl3/";
		shaderFragExt = ".frag";
		shaderVertExt = ".vert";
	}
	else if (type == API::OPENGL4) {
		shaderPath = engineDir + "/shaders/glsl/gl4/";
		shaderFragExt = ".frag";
		shaderVertExt = ".vert";
	}

	m_light = api->CreateShader();
	m_light->compileShaderFromFile(
		shaderPath + "pbr/Light" + shaderVertExt,
		shaderPath + "pbr/Light" + shaderFragExt
	);

	m_beauty = api->CreateShader();
	m_beauty->compileShaderFromFile(
		shaderPath + "Beauty" + shaderVertExt,
		shaderPath + "Beauty" + shaderFragExt
	);

	m_shadow = api->CreateShader();
	m_shadow->compileShaderFromFile(
		shaderPath + "Shadow" + shaderVertExt,
		shaderPath + "Shadow" + shaderFragExt
	);

	m_shadowBlur = api->CreateShader();
	m_shadowBlur->compileShaderFromFile(
		shaderPath + "ShadowBlur" + shaderVertExt,
		shaderPath + "ShadowBlur" + shaderFragExt
	);

	m_shadowCalc = api->CreateShader();
	m_shadowCalc->compileShaderFromFile(
		shaderPath + "ShadowCalc" + shaderVertExt,
		shaderPath + "ShadowCalc" + shaderFragExt
	);

	m_ssao = api->CreateShader();
	m_ssao->compileShaderFromFile(
		shaderPath + "pbr/SSAO" + shaderVertExt,
		shaderPath + "pbr/SSAO" + shaderFragExt
	);

	m_ssaoBlur = api->CreateShader();
	m_ssaoBlur->compileShaderFromFile(
		shaderPath + "pbr/SSAO_Blur" + shaderVertExt,
		shaderPath + "pbr/SSAO_Blur" + shaderFragExt
	);

	m_reflections = api->CreateShader();
	m_reflections->compileShaderFromFile(
		shaderPath + "pbr/SSR" + shaderVertExt,
		shaderPath + "pbr/SSR" + shaderFragExt
	);

	m_debug = api->CreateShader();
	m_debug->compileShaderFromFile(
		shaderPath + "Debug" + shaderVertExt,
		shaderPath + "Debug" + shaderFragExt
	);

	m_gbuff_handles = api->CreateShaderStorageBuffer(sizeof(uint64_t) * 7 * 2, 8);
	m_shadowmaps = api->CreateShaderStorageBuffer(sizeof(uint64_t) * 10 * 2, 16);
	m_sfx_handles = api->CreateShaderStorageBuffer(sizeof(uint64_t) * 5 * 2, 9);
	m_final_passes_handle = api->CreateShaderStorageBuffer(sizeof(uint64_t) * 5 * 2, 12);
}

void RD_RenderingPipelinePBR::CreateGBuff(RD_RenderingAPI* api) {
	// G-Buffer
	int w, h;
	w = api->GetWindowingSystem()->GetWidth();
	h = api->GetWindowingSystem()->GetHeight();

	m_gbuff = api->CreateFrameBuffer(w, h, false);
	m_gbuff->SetMultisampled(false);

	m_gbuff->AddAttachement(IMGFORMAT_RGB16F); // Position
	m_gbuff_struct.gPos = 0;

	m_gbuff->AddAttachement(IMGFORMAT_RGB16F); // Normal
	m_gbuff_struct.gNorm = 1;

	m_gbuff->AddAttachement(IMGFORMAT_RGBA); // Albedo
	m_gbuff_struct.gAlbedo = 2;

	m_gbuff->AddAttachement(IMGFORMAT_R16F); // Specular
	m_gbuff_struct.gSpec = 3;

	m_gbuff->AddAttachement(IMGFORMAT_RGB16F); // Metallic, Roughness, AO
	m_gbuff_struct.gMetRoughAO = 4;

	m_gbuff->AddAttachement(IMGFORMAT_RGB16F); // Emissive
	m_gbuff_struct.gEmissive = 5;

	m_gbuff->AddAttachement(IMGFORMAT_DEPTH, SCALEMODE_LINEAR); // Depth
	m_gbuff_struct.gDepth = 6;

	m_gbuff->BuildFBO();

	for (int i = 0; i < 7; i++) {
		m_gbuff->GetAttachementByIndex(i)->MakeTexBindless(api, m_gbuff_handles, i);
	}

	m_light_fb = api->CreateFrameBuffer(w, h, true);
	m_light_fb->AddAttachement(IMGFORMAT_RGB16F);
	m_light_fb->BuildFBO();
	m_light_fb->GetAttachementByIndex(0)->MakeTexBindless(api, m_final_passes_handle, 1);

	m_shadow_fb = api->CreateFrameBuffer(w, h, true);
	m_shadow_fb->AddAttachement(IMGFORMAT_RGB);
	m_shadow_fb->BuildFBO();
	m_shadow_fb->GetAttachementByIndex(0)->MakeTexBindless(api, m_sfx_handles, 0);

	m_shadow_blur_a = api->CreateFrameBuffer(w, h, true);
	m_shadow_blur_a->AddAttachement(IMGFORMAT_RGB);
	m_shadow_blur_a->BuildFBO();
	m_shadow_blur_a->GetAttachementByIndex(0)->MakeTexBindless(api, m_sfx_handles, 1);

	m_shadow_blur_b = api->CreateFrameBuffer(w, h, true);
	m_shadow_blur_b->AddAttachement(IMGFORMAT_RGB);
	m_shadow_blur_b->BuildFBO();
	m_shadow_blur_b->GetAttachementByIndex(0)->MakeTexBindless(api, m_sfx_handles, 2);

	m_final_passes = api->CreateFrameBuffer(w, h, true);
	m_final_passes->AddAttachement(IMGFORMAT_RGB);
	m_final_passes->BuildFBO();
	m_final_passes->GetAttachementByIndex(0)->MakeTexBindless(api, m_final_passes_handle, 0);
}

void RD_RenderingPipelinePBR::InitUBO(RD_RenderingAPI* api) {
	m_lspace_mats = api->CreateUniformBuffer(sizeof(float) * 16 * 10, 15);
	m_blurState = api->CreateUniformBuffer(sizeof(GLSL_BlurState), 10);
}

RD_ShaderStorageBuffer* RD_RenderingPipelinePBR::GetShadowMapBufferHandle() {
	return m_shadowmaps;
}

void RD_RenderingPipelinePBR::BlurShadow() {
	constexpr GLSL_BlurState st_a = { { 0.0f, 1.0f, 0.0f }, 0, 0 };
	constexpr GLSL_BlurState st_b = { { 1.0f, 0.0f, 0.0f }, 1, 0 };

	m_shadowBlur->useShader();

	if (m_shadow_fb->GetAttachementByIndex(0)->BindTexture(0)) {
		m_shadowBlur->SetInt("baseImage", 0);
	}

	m_shadow_blur_a->BindFBO();

	m_blurState->BindBuffer();
	m_blurState->SetBufferSubData(0, sizeof(GLSL_BlurState), (void*)&st_a);
	m_blurState->UnbindBuffer();

	m_quad->RenderQuad();

	m_shadow_blur_b->BindFBO();

	m_blurState->BindBuffer();
	m_blurState->SetBufferSubData(0, sizeof(GLSL_BlurState), (void*)&st_b);
	m_blurState->UnbindBuffer();

	m_quad->RenderQuad();

	m_shadow_blur_b->UnbindFBO();
}

void RD_RenderingPipelinePBR::ResizeFramebuffers(RD_RenderingAPI* api, const int w, const int h) {
	m_gbuff->ChangeFramebufferSize(w, h);
	m_light_fb->ChangeFramebufferSize(w, h);
	m_shadow_fb->ChangeFramebufferSize(w, h);
	m_shadow_blur_a->ChangeFramebufferSize(w, h);
	m_shadow_blur_b->ChangeFramebufferSize(w, h);
	m_final_passes->ChangeFramebufferSize(w, h);

	for (int i = 0; i < 7; i++) {
		m_gbuff->GetAttachementByIndex(i)->MakeTexBindless(api, m_gbuff_handles, i);
	}

	m_light_fb->GetAttachementByIndex(0)->MakeTexBindless(api, m_final_passes_handle, 1);
	m_shadow_fb->GetAttachementByIndex(0)->MakeTexBindless(api, m_sfx_handles, 0);
	m_shadow_blur_a->GetAttachementByIndex(0)->MakeTexBindless(api, m_sfx_handles, 1);
	m_shadow_blur_b->GetAttachementByIndex(0)->MakeTexBindless(api, m_sfx_handles, 2);
	m_final_passes->GetAttachementByIndex(0)->MakeTexBindless(api, m_final_passes_handle, 0);
}

RD_ShaderLoader* RD_RenderingPipelinePBR::DebugStart() {
	m_debug->useShader();
	m_gbuff->DebugMode();

	return m_debug;
}