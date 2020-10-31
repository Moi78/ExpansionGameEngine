#include "pch.h"
#include "RD_PostProcess.h"

RD_PostProcessEffect::RD_PostProcessEffect(RD_ShaderLoader* shader) {
	m_shader = shader;
	m_screen = std::make_unique<RD_Quad>();
	m_screen->Bufferize();
}

RD_PostProcessEffect::~RD_PostProcessEffect() {
	delete m_shader;
}

void RD_PostProcessEffect::RenderEffect(int screenID) {
	m_shader->useShader();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenID);
	m_shader->SetInt("screen", 0);

	m_screen->RenderQuad();
}