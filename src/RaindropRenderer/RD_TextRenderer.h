#pragma once

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RD_API __declspec(dllexport)
	#else
		#define RD_API __declspec(dllimport)
	#endif //RAINDROPRENDERER_EXPORTS
#else
	#define RD_API
#endif //_WIN32

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <string>
#include <array>
#include <fstream>

#include "RaindropRenderer.h"
#include "RD_Texture.h"
#include "RD_RenderingAPI.h"

#include <vec2.h>

class RD_API RD_TextRenderer
{
public:
	RD_TextRenderer(RaindropRenderer* rndr);
	~RD_TextRenderer();

	bool LoadFont(const std::string& path, const int fontsize);

	RD_Texture* GetGlyphTexture(const char chr);
	vec2f GetGlyphRelativeMetrics(const char chr);
	int GetGlyphAdvance(const char chr);
private:
	RaindropRenderer* m_rndr;

	stbtt_fontinfo m_font;

	std::array<std::pair<RD_Texture*, vec2f>, 94> m_characters;atta 
};

