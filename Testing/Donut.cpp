#include "Donut.h"

Donut::Donut(EXP_Game* gameinstance, vec3f spawn) : EXP_Actor(gameinstance, spawn, vec3f(), vec3f(), CL_VDFUNCPTR(Donut::Tick), CL_VDFUNCPTR(Donut::OnStart)) {
	RD_Texture* tex = new RD_Texture();
	tex->LoadTexture("Content/Donut/tex.png");

	RD_Texture* spec = new RD_Texture();
	spec->GenerateColorTex(vec3f(0.1f, 0.1f, 0.1f));

	BD_MatDef mdonut = {};
	mdonut.BaseColor = tex->GetTextureID();
	mdonut.Shininess = 2.0f;
	mdonut.Specular = spec->GetTextureID();

	m_donut = new EXP_StaticMesh(m_game, "/Donut/donut", mdonut, spawn, vec3f(60.0f, 70.0f), vec3f(0.005f, 0.005f, 0.005f));
}

Donut::~Donut() {
	delete m_donut;
}

void Donut::OnStart() {

}

void Donut::Tick() {
	m_donut->GetRawMeshData()->addRotation(vec3f(0.0f, 0.1f, 0.0f));
}