#include "pch.h"
#include "RD_Font.h"

RD_Font::RD_Font(RD_FontRenderer* rndr, std::string fontName, std::string fontPath, int fontSize) {
	m_fontSize = fontSize;
	m_fontName = fontName;

	if (LoadFont(rndr->GetFTLib(), fontPath)) {
		std::cout << "Loaded font " << fontName << "." << std::endl;
		LoadGlyphs();
	}
	else {
		std::cerr << "ERROR: Cannot load font \"" << m_fontName << "\"." << std::endl;
	}

	FT_Done_Face(m_fontFace);
}

RD_Font::~RD_Font() {

}

bool RD_Font::LoadFont(FT_Library lib, std::string fontPath) {
	if (FT_New_Face(lib, fontPath.c_str(), 0, &m_fontFace)) {
		return false;
	}

	FT_Set_Pixel_Sizes(m_fontFace, 0, m_fontSize);
	return true;
}

std::string RD_Font::GetFontName() {
	return m_fontName;
}

void RD_Font::LoadGlyphs() {
	std::cout << "Loading glyphs for font " << m_fontName << "." << std::endl;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char chr = 0; chr < 128; chr++) {
		if (FT_Load_Char(m_fontFace, chr, FT_LOAD_RENDER)) {
			std::cerr << "ERROR: Cannot load glyph " << chr << std::endl;
			continue;
		}

		unsigned int tex;
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_fontFace->glyph->bitmap.width, m_fontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_fontFace->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Glyph glyph = {
			chr,
			tex,
			vec2f(m_fontFace->glyph->bitmap.width, m_fontFace->glyph->bitmap.rows),
			vec2f(m_fontFace->glyph->bitmap_left, m_fontFace->glyph->bitmap_top),
			m_fontFace->glyph->advance.x
		};
		m_glyphs.push_back(glyph);
	}
}

Glyph RD_Font::GetGlyph(char chr) {
	for (int i = 0; i < m_glyphs.size(); i++) {
		if (m_glyphs[i].glyph == chr) {
			return m_glyphs[i];
		}
	}

	return {0, 0, vec2f(0.0f, 0.0f), vec2f(0.0f, 0.0f), 0};
}