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

#include <memory>

#include "RD_ShaderLoader.h"
#include "RD_Quad.h"

class RD_API RD_PostProcessEffect
{
public:
	RD_PostProcessEffect(RaindropRenderer* rndr);
	~RD_PostProcessEffect();

	void RenderEffect(RD_Texture* screen);

	void LoadShader(RD_ShaderMaterial* shader);

private:
	RD_ShaderMaterial* m_shader;
	std::unique_ptr<RD_Quad> m_screen;
};

