#include "pch.h"
#include "RD_DirLight.h"

RD_DirLight::RD_DirLight(RaindropRenderer* rndr, vec3f dir, vec3f color, float brightness) : m_dir(dir), m_color(color), m_brightness(brightness), m_lspace(1.0f) {
	SetUpShadowFB(rndr, 4096);
	m_shadowCaster = true;

	m_far_plane = 100.0f;
	m_near_plane = -10.0f;

	m_rndr = rndr;
}

RD_DirLight::~RD_DirLight() {
	delete m_fbo;
}

void RD_DirLight::SetLightBrightness(float nBrightness) {
	m_brightness = nBrightness;
}

void RD_DirLight::SetLightColor(vec3f nColor) {
	m_color = nColor;
}

void RD_DirLight::SetLightDir(vec3f nDir) {
	m_dir = nDir;
}

float RD_DirLight::GetBrightness() {
	return m_brightness;
}

vec3f RD_DirLight::GetLightColor() {
	return m_color;
}

vec3f RD_DirLight::GetLightDir() {
	return m_dir;
}

void RD_DirLight::DepthRender(
	RD_RenderingAPI* rndr,
	RD_ShaderLoader* shadowShader,
	RD_MaterialLibrary* sceneData,
	vec3f CamPos
) {
	if (!m_shadowCaster)
		return;

	mat4f lightProj = ProjOrtho(-10.0f, 10.0f, -10.0f, 10.0f, m_near_plane, m_far_plane);

	vec3f fpos = (m_dir * -2) + CamPos;
	mat4f lightView = LookAt<float>(fpos, CamPos, vec3f(0.0f, 0.0f, 1.0f));

	m_rndr->PushLightProjView(lightProj, lightView);
	m_lspace = lightProj * lightView;

	rndr->SetViewportSize(m_shadowQuality, m_shadowQuality, 0, 0);

	m_fbo->BindFBO();
	rndr->Clear(DEPTH_BUFFER);

	for (auto m : sceneData->GetRawVector()) {
		m.second->DrawMeshesShadow();
	}

	m_fbo->UnbindFBO();

	rndr->SetViewportSize(
		rndr->GetWindowingSystem()->GetWidth(),
		rndr->GetWindowingSystem()->GetHeight(), 0, 0
	);
	
	rndr->Clear(DEPTH_BUFFER | COLOR_BUFFER);
}

void RD_DirLight::SetUpShadowFB(RaindropRenderer* rndr, unsigned int shadowQual) {
	m_shadowQuality = shadowQual;
	m_fbo = rndr->GetRenderingAPI()->CreateFrameBuffer(m_shadowQuality, m_shadowQuality, false);

	m_fbo->AddAttachement(IMGFORMAT_DEPTH, SCALEMODE_LINEAR);
	m_fbo->BuildFBO();

	RD_RenderingAPI* api = rndr->GetRenderingAPI();
	RD_RenderingPipeline* pline = rndr->GetRenderingPipeline();
	m_fbo->GetAttachementByIndex(0)->MakeTexBindless(api, pline->GetShadowMapBufferHandle(), rndr->GetDirLightsCount());
}

mat4f RD_DirLight::GetLightSpace() const {
	return m_lspace;
}

RD_Texture* RD_DirLight::GetDepthTexID() const {
	return m_fbo->GetAttachementByIndex(0);
}

void RD_DirLight::Cleanup(RaindropRenderer* rndr) {

}

void RD_DirLight::SetShadowCasting(bool scast) {
	m_shadowCaster = scast;
}

bool RD_DirLight::GetShadowCasting() const {
	return m_shadowCaster;
}

void RD_DirLight::SetNearFarPlanes(float nearp, float farp) {
	m_near_plane = nearp;
	m_far_plane = farp;
}

void RD_DirLight::SendOnBuffer(RD_UniformBuffer* ubo, int idx, int offset) {
	GLSLDirLight dlight_data = {
		{ m_dir.getX(), m_dir.getY(), m_dir.getZ() },
		0,
		{ m_color.getX(), m_color.getY(), m_color.getZ() },
		m_brightness
	};

	ubo->BindBuffer();
	ubo->SetBufferSubData(idx * sizeof(GLSLDirLight) + offset, sizeof(GLSLDirLight), (void*)&dlight_data);
	ubo->UnbindBuffer();
}