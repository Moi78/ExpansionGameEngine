#pragma once

#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <EXP_Camera.h>
#include <EXP_InputHandler.h>
#include <EXP_Callbacks.h>
#include <EXP_InstanciatedMesh.h>
#include <EXP_StaticMesh.h>
#include <EXP_MeshBuilder.h>
#include <EXP_RigidBody.h>
#include <EXP_Particles.h>

#include <RD_RenderingAPI.h>

#include <GLFW/glfw3.h>

class Character : public EXP_Actor
{
public:
	Character(EXP_Game* game);
	~Character();

	virtual void OnStart() override;
	virtual void OnTick() override;

	void MoveForward();
	void Roll();
	void PrnPos();

	void DestroyActor();

private:
	EXP_Camera* m_cam;

	EXP_RB_Capsule* m_bound;
	
	EXP_KeyboardCallback* m_move;
	EXP_KeyboardCallback* m_destroy;
	EXP_KeyboardCallback* m_roll;
	EXP_KeyboardCallback* m_getpos;

	EXP_StaticMesh* m_test;
};

