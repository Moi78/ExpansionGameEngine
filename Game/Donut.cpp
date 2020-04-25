#include "pch.h"
#include "Donut.h"

Donut::Donut(EXP_Game* game, vec3f spawn) : EXP_Actor(game, spawn, vec3f(), vec3f(), CL_VDFUNCPTR(Donut::OnTick), CL_VDFUNCPTR(Donut::OnStart), CL_VDFUNCPTR(Donut::OnUnregister)) {
	RD_Texture* bc = new RD_Texture();
	bc->LoadTexture("Content/Donut/tex.png");

	RD_Texture* w = new RD_Texture();
	w->GenerateColorTex(vec3f(1.0f, 1.0f, 1.0f));

	BD_MatDef mdef = {};
	mdef.BaseColor = bc->GetTextureID();
	mdef.Specular = w->GetTextureID();
	mdef.Shininess = 2.0f;

	m_donut = new EXP_StaticMesh(game, "/Donut/donut", mdef, spawn, vec3f(60.0f, 70.0f), vec3f(0.005f, 0.005f, 0.005f));
}

Donut::~Donut() {

}

void Donut::OnStart() {

}

void Donut::OnTick() {
	m_donut->GetRawMeshData()->addRotation(vec3f(0.0f, 0.1f, 0.0f));
}

void Donut::OnUnregister() {

}