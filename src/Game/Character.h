#pragma once

#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <EXP_Camera.h>
#include <EXP_InputHandler.h>
#include <EXP_Callbacks.h>
#include <EXP_InstanciatedMesh.h>
#include <EXP_MeshBuilder.h>

#include <EXP_RigidBody.h>

#include <GLFW/glfw3.h>

class Character : public EXP_Actor
{
public:
	Character(EXP_Game* game);
	~Character();

	virtual void Start() override;
	virtual void Tick() override;

	void MoveForward();
	void Roll();

	void DestroyActor();

private:
	EXP_Camera* m_cam;

	EXP_RB_Box* m_bound;

	EXP_KeyboardCallback* m_move;
	EXP_KeyboardCallback* m_destroy;
	EXP_KeyboardCallback* m_roll;
};

