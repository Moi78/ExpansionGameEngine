#pragma once

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RD_API __declspec(dllexport)
	#else
		#define RD_API __declspec(dllimport)
	#endif
#else
	#define RD_API
#endif //_WIN32

enum RD_API RendererFeature {
	Specular, Lighting, Ambient
};

enum class RD_API RenderMode {
	Wireframe, Filled
};

struct RD_API Gbuff {
	unsigned int gRBO;

	unsigned int gPos;
	unsigned int gAlbedo;
	unsigned int gSpec;
	unsigned int gShadows;

	unsigned int gNorm;

	unsigned int gLight; //Special tex, not used at the same time.
};