#include "pch.h"
#include "EXP_Callbacks.h"

EXP_KeyboardCallback::EXP_KeyboardCallback(EXP_Game* gameinstance, std::function<void()> func, int key, bool waitRelease) : RD_Callback(func) {
	m_key = key;
	m_gameinstance = gameinstance;
	m_waitRealease = waitRelease;
	m_released = true;

	m_gameinstance->RegisterKeyboardCallback(this);
}

EXP_KeyboardCallback::~EXP_KeyboardCallback() {
	m_gameinstance->UnregisterKeyboardCallback(this);
}

void EXP_KeyboardCallback::UpdateCallback() {
	if (m_waitRealease) {
		if (glfwGetKey(m_gameinstance->GetRenderer()->GetGLFWwindow(), m_key) == GLFW_PRESS) {
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
		if (glfwGetKey(m_gameinstance->GetRenderer()->GetGLFWwindow(), m_key) == GLFW_PRESS) {
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

	m_game->RegisterMouseButtonCallback(this);
}

EXP_MouseButtonCallback::~EXP_MouseButtonCallback() {

}

void EXP_MouseButtonCallback::UpdateCallback() {
	if (m_waitRelease) {
		if (glfwGetMouseButton(m_game->GetRenderer()->GetGLFWwindow(), m_btn) == GLFW_PRESS) {
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
		if (glfwGetMouseButton(m_game->GetRenderer()->GetGLFWwindow(), m_btn) == GLFW_PRESS) {
			Call();
		}
	}
}