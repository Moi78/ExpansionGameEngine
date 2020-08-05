#pragma once

#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <EXP_Camera.h>
#include <EXP_InputHandler.h>
#include <EXP_Callbacks.h>

class Character : public EXP_Actor
{
public:
	Character(EXP_Game* game);
	~Character();

	virtual void Start() override;
	virtual void Tick() override;

	void MoveForward();

private:
	EXP_Camera* m_cam;

	EXP_KeyboardCallback* m_move;
};

