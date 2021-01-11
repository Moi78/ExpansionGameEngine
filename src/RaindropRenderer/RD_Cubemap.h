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

#include "RaindropRenderer.h"
#include "stb/stb_image.h"

#include <vec2.h>

class RD_API RD_Cubemap
{
public:
	RD_Cubemap() {}
	virtual ~RD_Cubemap() {}

	virtual void BuildCubemapFromImages(std::array<std::string, 6> face_filepath, const bool flipTex = true) = 0;
	virtual void BindTexture(const int tex_unit) = 0;
};

#ifdef BUILD_OPENGL

class RD_API RD_Cubemap_GL :
	public RD_Cubemap
{
public:
	RD_Cubemap_GL();
	virtual ~RD_Cubemap_GL() override;

	virtual void BuildCubemapFromImages(std::array<std::string, 6> face_filepath, const bool flipTex = true);
	virtual void BindTexture(const int tex_unit);

private:
	unsigned int m_texture;
};

#endif //BUILD_OPENGL