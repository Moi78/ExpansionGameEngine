#pragma once

#ifndef _EXP_CALLBACKS_H__
#define _EXP_CALLBACKS_H__

#ifdef _WIN32

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#else

#define EXPGE_API

#endif //_WIN32

#include <RD_Callback.h>
#include <RaindropRenderer.h>

#include "EXP_Game.h"

class EXPGE_API EXP_KeyboardCallback : public RD_Callback {
public:
	EXP_KeyboardCallback(EXP_Game* gameinstance, std::function<void()> func, int key, bool waitRelease = false);
	~EXP_KeyboardCallback();

	void UpdateCallback();
private:
	int m_key;

	bool m_waitRealease;
	bool m_released;

	EXP_Game* m_gameinstance;
};

class EXPGE_API EXP_MouseButtonCallback : public RD_Callback {
public:
	EXP_MouseButtonCallback(EXP_Game* game, std::function<void()> func, int mousebtn, bool waitRelease = false);
	~EXP_MouseButtonCallback();
	
	void UpdateCallback();
private:
	EXP_Game* m_game;

	int m_btn;

	bool m_waitRelease;
	bool m_released;
};

#endif // !_EXP_CALLBACKS_H__
