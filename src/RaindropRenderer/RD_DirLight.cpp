#include "pch.h"
#include "RD_DirLight.h"

RD_DirLight::RD_DirLight(RaindropRenderer* rndr, vec3f dir, vec3f color, float brightness) : m_dir(dir), m_color(color), m_brightness(brightness), m_lspace(1.0f) {
	SetUpShadowFB(rndr, 4096);
	m_shadowCaster = true;
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

void RD_DirLight::DepthRender(RaindropRenderer* rndr, vec3f CamPos) {
	if (!m_shadowCaster)
		return;

	rndr->SwitchShader(rndr->GetShadowShader());

	mat4f lightProj = ProjOrtho(-30.0f, 30.0f, -30.0f, 30.0f, -10.0f, 100.0f);

	vec3f fpos = (m_dir * -5) + CamPos;
	mat4f lightView = LookAt<float>(fpos, CamPos, vec3f(0.0f, 0.0f, 1.0f));

	rndr->PushLightProjViewMatrices(lightView, lightProj);
	m_lspace = lightProj * lightView;

	rndr->GetRenderingAPI()->SetViewportSize(m_shadowQuality, m_shadowQuality, 0, 0);

	m_fbo->BindFBO();
	rndr->GetRenderingAPI()->Clear(DEPTH_BUFFER);

	rndr->RenderShadowMeshes();

	m_fbo->UnbindFBO();

	rndr->GetRenderingAPI()->SetViewportSize(rndr->GetViewportSize().getX(), rndr->GetViewportSize().getY(), 0, 0);
	rndr->GetRenderingAPI()->Clear(DEPTH_BUFFER | COLOR_BUFFER);
}

void RD_DirLight::SetUpShadowFB(RaindropRenderer* rndr, unsigned int shadowQual) {
	m_shadowQuality = shadowQual;
	m_fbo = rndr->GetRenderingAPI()->CreateFrameBuffer(m_shadowQuality, m_shadowQuality, false);

	m_fbo->AddAttachement(IMGFORMAT_DEPTH, SCALEMODE_LINEAR);
	m_fbo->BuildFBO();

	m_fbo->GetAttachementByIndex(0)->MakeTexBindless(rndr, rndr->GetShadowMapsBufferHandle(), rndr->GetDirLightsCount());
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
