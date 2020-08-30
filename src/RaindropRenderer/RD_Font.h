#pragma once

#ifdef _WIN32
#ifdef RAINDROPRENDERER_EXPORTS
#define RAINDROPRENDERER_API __declspec(dllexport)
#else
#define RAINDROPRENDERER_API __declspec(dllimport)
#endif //RAINDROPRENDERER_EXPORTS
#else
#define RAINDROPRENDERER_API
#endif //_WIN32

#include <ft2build.h>
#include FT_FREETYPE_H

#include "RD_FontRenderer.h"

#include <iostream>
#include <string>

struct Glyph {
	char glyph;
	unsigned int TexId;
	vec2f Size;
	vec2f Bearing;
	unsigned int Advance;
};

class RAINDROPRENDERER_API RD_Font
{
public:
	RD_Font(RD_FontRenderer* rndr, std::string fontName, std::string fontPath, int fontSize = 12);
	~RD_Font();

	std::string GetFontName();
	
	Glyph GetGlyph(char chr);

private:
	bool LoadFont(FT_Library lib, std::string fontPath);
	void LoadGlyphs();

	FT_Face m_fontFace;
	int m_fontSize;

	std::string m_fontName;

	std::vector<Glyph> m_glyphs;
};

