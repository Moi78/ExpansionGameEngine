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

#include "RD_RenderingAPI.h"

class RAINDROPRENDERER_API RD_Quad
{
public:
	RD_Quad(RD_RenderingAPI* rndr);
	~RD_Quad();

	void RenderQuad();
	virtual void Bufferize();

protected:
	RD_RenderingAPI* m_api;
	RD_RenderingAPI_VertexBuffer* m_vbuff;
};

#endif