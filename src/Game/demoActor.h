#pragma once

#ifndef _DEMO_ACTOR_H__
#define _DEMO_ACTOR_H__

#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <EXP_RigidBody.h>
#include <EXP_Camera.h>
#include <EXP_InputHandler.h>
#include <EXP_Callbacks.h>

class demoActor : public EXP_Actor
{
public:
	demoActor(EXP_Game* game, vec3f spawn);
	~demoActor();

	void Tick();
	void Start();
	void Unregister();

	void MoveForward();
	void Kaboom();

private:
	EXP_Camera* m_cam;
	EXP_RB_Box* m_bound;

	EXP_KeyboardCallback* m_moveforwd;
	EXP_KeyboardCallback* m_kaboom;
};

#endif //_DEMO_ACTOR_H__