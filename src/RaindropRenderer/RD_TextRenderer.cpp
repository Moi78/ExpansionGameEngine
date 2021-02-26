#include "pch.h"
#include "RD_TextRenderer.h"

RD_TextRenderer::RD_TextRenderer(RaindropRenderer* rndr) {
	m_rndr = rndr;
}

RD_TextRenderer::~RD_TextRenderer() {
	for (auto tex : m_characters) {
		delete tex.first;
	}
}

bool RD_TextRenderer::LoadFont(const std::string& path, const int fontsize) {
	std::ifstream file;
	file.open(path, std::ios::ate | std::ios::binary);

	const size_t fsize = file.tellg();
	std::vector<char> buffer(fsize);

	file.seekg(0);
	file.read(buffer.data(), fsize);

	file.close();

	int res = stbtt_InitFont(
		&m_font, reinterpret_cast<unsigned char*>(buffer.data()),
		stbtt_GetFontOffsetForIndex(reinterpret_cast<unsigned char*>(buffer.data()), 0)
	);

	if (!res) {
		std::cerr << "ERROR: Failed to init font " << path << std::endl;
		return false;
	}

	for (int i = 0; i < 94; i++) {
		int w, h;
		unsigned char* bmp = stbtt_GetCodepointBitmap(&m_font, 0, stbtt_ScaleForPixelHeight(&m_font, fontsize), i + 33, &w, &h, NULL, NULL);

		m_characters[i].first = m_rndr->GetRenderingAPI()->CreateTexture();
		m_characters[i].first->CreateTextureFromGlyph(bmp, w, h);

		m_characters[i].second = vec2f((float)w, (float)h);
	}

	return true;
}

RD_Texture* RD_TextRenderer::GetGlyphTexture(const char chr) {
	return m_characters[chr - 33].first;
}

vec2f RD_TextRenderer::GetGlyphRelativeMetrics(const char chr) {
	return m_characters[chr - 33].second;
}

int RD_TextRenderer::GetGlyphAdvance(const char chr) {
	int adv;
	stbtt_GetCodepointHMetrics(&m_font, chr, &adv, NULL);

	return adv;
}