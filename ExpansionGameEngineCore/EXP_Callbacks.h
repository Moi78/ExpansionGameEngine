#pragma once

#ifndef _EXP_CALLBACKS_H__
#define _EXP_CALLBACKS_H__

#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#include <RD_Callback.h>
#include <RaindropRenderer.h>

#include "EXP_Game.h"

class EXPGE_API EXP_KeyboardCallback : public RD_Callback {
public:
	EXP_KeyboardCallback(EXP_Game* gameinstance, std::function<void()> func, int key);
	~EXP_KeyboardCallback();

	void UpdateCallback();
private:
	int m_key;

	EXP_Game* m_gameinstance;
};

#endif // !_EXP_CALLBACKS_H__
