#include "pch.h"
#include "EXP_Callbacks.h"

EXP_KeyboardCallback::EXP_KeyboardCallback(EXP_Game* gameinstance, std::function<void()> func, int key) : RD_Callback(func) {
	m_key = key;
	m_gameinstance = gameinstance;

	m_gameinstance->RegisterKeyboardCallback(this);
}

EXP_KeyboardCallback::~EXP_KeyboardCallback() {

}

void EXP_KeyboardCallback::UpdateCallback() {
	if (glfwGetKey(m_gameinstance->GetRenderer()->GetGLFWwindow(), m_key) == GLFW_PRESS) {
		Call();
	}
}