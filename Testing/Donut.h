#pragma once

#ifndef _DONUT_H__
#define _DONUT_H__

#include <EXP_Actor.h>
#include <EXP_StaticMesh.h>
#include <EXP_Game.h>
#include <EXP_RigidBody.h>

#include <BD_StructMan.h>

#include <RD_Texture.h>

#include <vec3.h>

class Donut : public EXP_Actor
{
public:
	Donut(EXP_Game* gameinstance, vec3f spawn);
	~Donut();

	void OnStart();
	void Tick();
	void OnUnregister();

private:
	EXP_StaticMesh* m_donut;
};

#endif //_DONUT_H__