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

#include <cstring>

class RAINDROPRENDERER_API RD_Quad
{
public:
	RD_Quad();
	~RD_Quad();

	void RenderQuad();

private:
	void Bufferize();

	unsigned int m_vao;
	unsigned int m_vbo;
};

#endif