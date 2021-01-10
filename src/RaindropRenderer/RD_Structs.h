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
	Specular, Lighting, Ambient, SSAO
};

enum class RD_API RenderMode {
	Wireframe, Filled
};

struct RD_API Gbuff {
	unsigned int gRBO;

	unsigned int gPos;
	unsigned int gNorm;
	unsigned int gAlbedo;
	unsigned int gSpec;
	unsigned int gShadows; //Deprecated
	unsigned int gDepth;
	
	//PBR Stuff
	unsigned int gMetRoughAO;

	//Special tex, not used at the same time.
	unsigned int gLight;
};