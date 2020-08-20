#pragma once

#ifdef _WIN32
	#ifdef RAINDROPRENDERER_EXPORTS
		#define RAINDROPRENDERER_API __declspec(dllexport)
	#else
		#define RAINDROPRENDERER_API __declspec(dllimport)
	#endif //RAINDROPRENDERER_EXPORTS

#else

#define RAINDROPRENDERER_API

#endif //_WIN32

#ifndef RD_SIZED_QUAD_H__
#define RD_SIZED_QUAD_H__

#include "RaindropRenderer.h"
#include "RD_Quad.h"

class RAINDROPRENDERER_API RD_SizedQuad : public RD_Quad
{
public:
	RD_SizedQuad(float sizex, float sizey, float offsetx = 0.0f, float offsety = 0.0f);
	~RD_SizedQuad();

	virtual void Bufferize() override;
private:

	float m_sizex, m_sizey;
	float m_offsetx, m_offsety;
};

#endif //RD_SIZED_QUAD_H__