#pragma once

#include <EXP_Game.h>
#include <EXP_Camera.h>
#include <EXP_Actor.h>

#include <EXP_PhysicsHandler.h>
#include <EXP_RigidBody.h>

#include <vec3.h>

class MainCharacter : public EXP_Actor
{
public:
	MainCharacter(EXP_Game* gameinstance, vec3f spawnPoint);
	~MainCharacter();

	void Tick();
	void OnLoad();

	void MoveForward();

private:
	EXP_Camera* m_camera;
	EXP_RB_Box* m_bound;

	EXP_KeyboardCallback* m_forwd;

	EXP_Game* m_gameinstance;
};

