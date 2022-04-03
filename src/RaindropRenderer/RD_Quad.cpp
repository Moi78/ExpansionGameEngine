#include "pch.h"
#include "RD_Quad.h"

RD_Quad::RD_Quad(RD_RenderingAPI* api) {
	m_api = api;

	m_vbuff = api->CreateVertexBuffer();
}

RD_Quad::~RD_Quad() {
	delete m_vbuff;
}

void RD_Quad::Bufferize() {
	float geom[32] = {
	-1.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f,    0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f,    0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,     0.0f, 0.0f, 0.0f,    1.0f, 1.0f,
	1.0f, -1.0f, 0.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f
	};

	m_vbuff->CreateBuffer();
	m_vbuff->FillBufferData(&geom[0], 32);
}

void RD_Quad::RenderQuad() {
	m_vbuff->BindBuffer();
	m_api->DrawVB(m_vbuff, DrawMode::TRIANGLES_STRIP);
	m_vbuff->UnbindBuffer();
}