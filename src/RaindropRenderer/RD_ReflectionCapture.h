#pragma once

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RAINDROPRENDERER_API __declspec(dllexport)
	#else
		#define RAINDROPRENDERER_API __declspec(dllimport)
	#endif //RAINDROPRENDERER_EXPORTS
#endif //_WIN32

#include "RaindropRenderer.h"
#include "RD_FrameBuffer.h"
#include "RD_Cubemap.h"
#include "RD_Camera.h"

#include <array>

class RAINDROPRENDERER_API RD_ReflectionCapture
{
public:
	RD_ReflectionCapture(RaindropRenderer* rndr, vec3f capturePoint);
	~RD_ReflectionCapture();

	void RenderCapture();

private:
	RaindropRenderer* m_rndr;
	vec3f m_capPoint;

	std::array<RD_FrameBuffer*, 6> m_fbs;
	RD_Cubemap* m_capEnv;

	RD_Camera* m_cam;
};

