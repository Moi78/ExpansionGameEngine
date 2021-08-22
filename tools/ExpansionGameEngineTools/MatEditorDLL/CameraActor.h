#pragma once

#include <EXP_Game.h>
#include <EXP_Actor.h>
#include <EXP_Camera.h>
#include <EXP_InputHandler.h>
#include <EXP_Callbacks.h>

#include <RD_RenderingAPI.h>

#include <GLFW/glfw3.h>

class CameraActor : public EXP_Actor
{
public:
	CameraActor(EXP_Game* game);
	~CameraActor();

	virtual void OnStart() override;
	virtual void OnTick() override;

private:
	EXP_Camera* m_cam;
};

