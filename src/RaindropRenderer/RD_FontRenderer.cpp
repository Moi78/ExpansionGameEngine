#include "pch.h"
#include "RD_FontRenderer.h"

#include "RD_Font.h"

RD_FontRenderer::RD_FontRenderer() {
	
}

RD_FontRenderer::~RD_FontRenderer() {

}

bool RD_FontRenderer::InitFontRenderer() {
	if (FT_Init_FreeType(&m_lib)) {
		std::cerr << "ERROR: Cannot init FreeType Library" << std::endl;
		return false;
	}
	else {
		std::cout << "FreeType successfully initilized." << std::endl;
		return true;
	}
}

FT_Library RD_FontRenderer::GetFTLib() {
	return m_lib;
}

void RD_FontRenderer::RegisterFont(RD_Font* font) {
	std::cout << "Registering new font" << std::endl;
	m_fonts.push_back(font);
}

void RD_FontRenderer::UnregisterFont(RD_Font* font) {
	std::cout << "Unregistering font." << std::endl;

	int index = GetElemIndex<RD_Font*>(m_fonts, font);

	if (index != -1) {
		m_fonts.erase(m_fonts.begin() + index);

		if(font)
			delete font;
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

RD_Texture* RD_FontRenderer::RenderTextToTex(std::string fontName, std::string text) {
	RD_Font* font = nullptr;
	for (auto ft : m_fonts) {
		if (ft->GetFontName() == fontName) {
			font = ft;
		}
	}

	if (!font) {
		std::cerr << "ERROR: Requested font not registered." << std::endl;
		return nullptr;
	}

	//Loading text
	unsigned int m_texs[sizeof(text)];

	return nullptr;
}