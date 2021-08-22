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
	Specular = 0,
	Lighting = 1,
	Ambient  = 2,
	SSAO	 = 3,
	Bloom	 = 4
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
	unsigned int gEmissive; 
	unsigned int gDepth;
	
	//PBR Stuff
	unsigned int gMetRoughAO;

	//Special tex, not used at the same time.
	unsigned int gLight;
};

enum RD_API API {
	OPENGL3,
	OPENGL4,
	VULKAN,
	DIRECTX
};