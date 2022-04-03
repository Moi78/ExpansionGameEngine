#include "pch.h"
#include "RD_PostProcess.h"

RD_PostProcessEffect::RD_PostProcessEffect(RaindropRenderer* rndr) {
	m_shader = nullptr;
	m_screen = std::make_unique<RD_Quad>(rndr->GetRenderingAPI());
	m_screen->Bufferize();
}

RD_PostProcessEffect::~RD_PostProcessEffect() {
	delete m_shader;
}

void RD_PostProcessEffect::RenderEffect(RD_Texture* screen) {
	m_shader->GetShader()->useShader();
	m_shader->BindMaterial();

	if (screen->BindTexture(9)) {
		m_shader->GetShader()->SetInt("screen", 9);
	}

	m_screen->RenderQuad();
}

void RD_PostProcessEffect::LoadShader(RD_ShaderMaterial* shader) {
	m_shader = shader;
}