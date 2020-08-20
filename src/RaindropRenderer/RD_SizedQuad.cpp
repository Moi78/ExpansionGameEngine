#include "pch.h"
#include "RD_SizedQuad.h"

RD_SizedQuad::RD_SizedQuad(float sx, float sy, float ox, float oy) : m_sizex(sx), m_sizey(sy), m_offsetx(ox), m_offsety(oy), RD_Quad() {

}

RD_SizedQuad::~RD_SizedQuad() {

}

void RD_SizedQuad::Bufferize() {
	float geom[20] = {
	m_offsetx,			 m_offsety,						0.0f,			0.0f, 1.0f,
	m_offsetx,			 -m_sizey + m_offsety,			0.0f,			0.0f, 0.0f,
	m_sizex + m_offsetx, m_offsety,						0.0f,			1.0f, 1.0f,
	m_sizex + m_offsetx, -m_sizey + m_offsety,			0.0f,			1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(geom), &geom, GL_STATIC_DRAW);

	//Vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	//UVs
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}