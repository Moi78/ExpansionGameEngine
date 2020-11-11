#include "pch.h"
#include "EXP_Callbacks.h"

EXP_KeyboardCallback::EXP_KeyboardCallback(EXP_Game* gameinstance, std::function<void()> func, int key, bool waitRelease) : RD_Callback(func) {
	m_key = key;
	m_gameinstance = gameinstance;
	m_waitRealease = waitRelease;
	m_released = true;

	m_win = gameinstance->GetRenderer()->GetRenderingAPI()->GetWindowingSystem();

	m_gameinstance->RegisterKeyboardCallback(this);
}

EXP_KeyboardCallback::~EXP_KeyboardCallback() {
	//m_gameinstance->UnregisterKeyboardCallback(this);
}

void EXP_KeyboardCallback::UpdateCallback() {
	if (m_waitRealease) {
		if (m_win->GetKeyPress(m_key)) {
			if (m_released) {
				m_released = false;
				Call();
			}
		}
		else {
			m_released = true;
		}
	}
	else {
		if (m_win->GetKeyPress(m_key)) {
			Call();
		}
	}
}

//----------------------------------------------------------------------------------------------------------

EXP_MouseButtonCallback::EXP_MouseButtonCallback(EXP_Game* game, std::function<void()> func, int mousebtn, bool waitRelease) : RD_Callback(func) {
	m_btn = mousebtn;
	m_waitRelease = waitRelease;
	m_released = false;

	m_game = game;
	m_win = game->GetRenderer()->GetRenderingAPI()->GetWindowingSystem();

	m_game->RegisterMouseButtonCallback(this);
}

EXP_MouseButtonCallback::~EXP_MouseButtonCallback() {

}

void EXP_MouseButtonCallback::UpdateCallback() {
	if (m_waitRelease) {
		if (m_win->GetMouseButton(m_btn)) {
			if (m_released) {
				m_released = false;
				Call();
			}
		}
		else {
			m_released = true;
		}
	}
	else {
		if (m_win->GetMouseButton(m_btn)) {
			Call();
		}
	}
}