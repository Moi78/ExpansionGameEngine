#pragma once

#ifndef _DONUT_H__
#define _DONUT_H__

#ifdef _WIN32
#ifdef GAME_EXPORTS
#define GAME_API __declspec(dllexport)
#else
#define GAME_API __declspec(dllimport)
#endif
#define GAME_API
#endif

#include <EXP_Game.h>
#include <EXP_StaticMesh.h>
#include <EXP_Actor.h>
#include <EXP_HotLoad.h>

#include <BD_StructMan.h>

class Donut : public EXP_Actor
{
public:
	Donut(EXP_Game*, vec3f);
	~Donut();

	void OnStart();
	void OnTick();
	void OnUnregister();

private:
	EXP_StaticMesh* m_donut;
};

#endif