#pragma once

#ifndef _DEMO_ACTOR_H__
#define _DEMO_ACTOR_H__

#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <EXP_RigidBody.h>
#include <EXP_Camera.h>
#include <EXP_InputHandler.h>
#include <EXP_PointLight.h>
#include <EXP_Callbacks.h>
#include <EXP_StaticMesh.h>

class demoActor : public EXP_Actor
{
public:
	demoActor(EXP_Game* game, vec3f spawn);
	~demoActor();

	virtual void Tick() override;
	virtual void Start() override;
	virtual void Unregister() override;

	void MoveForward();
	void Kaboom();

private:
	EXP_Camera* m_cam;
	EXP_RB_Box* m_bound;
	EXP_PointLight* m_plight;

	EXP_StaticMesh* m_sphere;

	EXP_KeyboardCallback* m_moveforwd;
	EXP_KeyboardCallback* m_kaboom;
};

#endif //_DEMO_ACTOR_H__