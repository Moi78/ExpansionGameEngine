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
	bool GetKey(int key);

	void RegisterMouseButtonCallback(EXP_MouseButtonCallback*);
	void UnregisterMouseButtonCallback(EXP_MouseButtonCallback*);
	bool GetMouseButton(int button);

	void UnregisterAllCallbacks();

	void CaptureCursor(bool state);

	void UpdateKeyboardInput();
	void UpdateMouseInput();

	double GetMouseXaxis();
	double GetMouseYaxis();
	void ResetPointer();

	double GetAbsoluteMousePosX();
	double GetAbsoluteMousePosY();

	double GetGUI_SpaceMousePosX();
	double GetGUI_SpaceMousePosY();

private:
	GLFWwindow* m_win;
	bool m_curHidden;

	//Callbacks
	std::vector<EXP_KeyboardCallback*> m_kb_callbacks;
	std::vector<EXP_MouseButtonCallback*> m_mouse_callbacks;
};

#endif //_EXP_INPUT_HANDLER_H__