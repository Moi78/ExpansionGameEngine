#include "pch.h"
#include "RD_PostProcess.h"

RD_PostProcessEffect::RD_PostProcessEffect() {
	m_shader = nullptr;
	m_screen = std::make_unique<RD_Quad>();
	m_screen->Bufferize();
}

RD_PostProcessEffect::~RD_PostProcessEffect() {
	delete m_shader;
}

void RD_PostProcessEffect::RenderEffect(unsigned int screenID) {
	m_shader->GetShader()->useShader();
	m_shader->BindMaterial();

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, screenID);
	m_shader->GetShader()->SetInt("screen", 9);

	m_screen->RenderQuad();
}

void RD_PostProcessEffect::LoadShader(RD_ShaderMaterial* shader) {
	m_shader = shader;
}