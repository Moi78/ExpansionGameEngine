#include "pch.h"
#include "RD_DirLight.h"

RD_DirLight::RD_DirLight(vec3f dir, vec3f color, float brightness) : m_dir(dir), m_color(color), m_brightness(brightness) {
	SetUpShadowFB(1024);
}

RD_DirLight::~RD_DirLight() {

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
	rndr->SwitchShader(rndr->GetShadowShader());

	glm::mat4 lightProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f);

	glm::mat4 lightView = glm::lookAt(
		glm::vec3(-m_dir.getX(), -m_dir.getY(), -m_dir.getZ()),
		glm::vec3(0),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);

	rndr->GetCurrentShader()->SetMatrix("lightproj", lightProj);
	rndr->GetCurrentShader()->SetMatrix("lightview", lightView);

	m_lspace = lightProj * lightView;

	glViewport(0, 0, m_shadowQuality, m_shadowQuality);

	glCullFace(GL_FRONT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	rndr->RenderShadowMeshes();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);

	glViewport(0, 0, rndr->getWindowWidth(), rndr->getWindowHeigh());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rndr->GetCurrentShader()->SetMatrix("lspaceMat", m_lspace);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_depthMapTEX);
	rndr->GetCurrentShader()->SetInt("ShadowMap", 1);
}

void RD_DirLight::SetUpShadowFB(unsigned int shadowQual) {
	m_shadowQuality = shadowQual;

	//Texture and Framebuffer generation
	glGenFramebuffers(1, &m_depthMapFBO);

	glGenTextures(1, &m_depthMapTEX);
	glBindTexture(GL_TEXTURE_2D, m_depthMapTEX);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowQuality, m_shadowQuality, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Framebuffer config
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMapTEX, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 RD_DirLight::GetLightSpace() {
	return m_lspace;
}

unsigned int RD_DirLight::GetDepthTexID() {
	return m_depthMapTEX;
}