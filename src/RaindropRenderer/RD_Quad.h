#pragma once

#ifndef _RD_QUAD_H__
#define _RD_QUAD_H__

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RAINDROPRENDERER_API __declspec(dllexport)
	#else
		#define RAINDROPRENDERER_API __declspec(dllimport)
	#endif
#else
		#define RAINDROPRENDERER_API
#endif

#include "RaindropRenderer.h"
#include "RD_RenderingAPI.h"

class RAINDROPRENDERER_API RD_Quad
{
public:
	RD_Quad(RaindropRenderer* rndr);
	~RD_Quad();

	void RenderQuad();
	virtual void Bufferize();

protected:
	RaindropRenderer* m_rndr;
	RD_RenderingAPI_VertexBuffer* m_vbuff;
};

#endif