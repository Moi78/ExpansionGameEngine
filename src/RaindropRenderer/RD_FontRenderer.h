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

#include "RaindropRenderer.h"
#include "RD_Texture.h"

#include <vec2.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <vector>
#include <string>

//Forward declaration
class RD_Font;

class RAINDROPRENDERER_API RD_FontRenderer
{
public:
	RD_FontRenderer();
	~RD_FontRenderer();

	bool InitFontRenderer();

	void RegisterFont(RD_Font* font);
	void UnregisterFont(RD_Font* font);

	RD_Texture* RenderTextToTex(std::string fontName, std::string text);

	FT_Library GetFTLib();

private:
	FT_Library m_lib;

	std::vector<RD_Font*> m_fonts;
};

