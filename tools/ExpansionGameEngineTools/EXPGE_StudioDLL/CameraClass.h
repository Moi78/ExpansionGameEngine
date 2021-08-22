#pragma once
#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <EXP_Camera.h>
#include <EXP_InputHandler.h>

class CameraClass : public EXP_Actor
{
public:
	CameraClass(EXP_Game* game);
	~CameraClass();

	virtual void OnStart();
	virtual void OnTick();

	void AllowMoving();
	void Forward();
	void Backward();
private:
	EXP_Camera* m_cam;

	EXP_MouseButtonCallback* m_mb_cbk;
	EXP_KeyboardCallback* m_forward;
	EXP_KeyboardCallback* m_backward;

	bool m_mvt_allowed;
};

