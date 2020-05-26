#pragma once

#ifndef _EXP_INPUT_HANDLER_H__
#define _EXP_INPUT_HANDLER_H__

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif
#endif //_WIN32

#include "EXP_Game.h"
#include "EXP_Callbacks.h"

class EXPGE_API EXP_InputHandler
{
public:
	EXP_InputHandler(GLFWwindow*);
	~EXP_InputHandler();

	void RegisterKeyboardCallback(EXP_KeyboardCallback*);

	void UnregisterKeyboardCallback(EXP_KeyboardCallback*);

	void CaptureCursor(bool state);
	void UpdateKeyboardInput();

private:
	GLFWwindow* m_win;

	//Callbacks
	std::vector<EXP_KeyboardCallback*> m_kb_callbacks;
};

#endif //_EXP_INPUT_HANDLER_H__