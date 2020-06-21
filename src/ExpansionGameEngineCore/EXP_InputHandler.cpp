#include "pch.h"
#include "EXP_InputHandler.h"

EXP_InputHandler::EXP_InputHandler(GLFWwindow* win) {
	m_win = win;
	m_curHidden = false;

	/*if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(win, GLFW_RAW_MOUSE_MOTION, GL_TRUE);
	}*/
}

EXP_InputHandler::~EXP_InputHandler() {

}

void EXP_InputHandler::RegisterKeyboardCallback(EXP_KeyboardCallback* kbcbk) {
	m_kb_callbacks.push_back(kbcbk);
}

void EXP_InputHandler::UnregisterKeyboardCallback(EXP_KeyboardCallback* kbcbk) {
	int index = GetElemIndex<EXP_KeyboardCallback*>(m_kb_callbacks, kbcbk);

	if (index != -1) {
		m_kb_callbacks.erase(m_kb_callbacks.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void EXP_InputHandler::CaptureCursor(bool state) {
	if (state) {
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else {
		glfwSetInputMode(m_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	m_curHidden = state;
}

void EXP_InputHandler::UpdateKeyboardInput() {
	for (auto cllbck : m_kb_callbacks) {
		cllbck->UpdateCallback();
	}
}

double EXP_InputHandler::GetMouseXaxis() {
	double xaxis;
	glfwGetCursorPos(m_win, &xaxis, NULL);

	return xaxis;
}

double EXP_InputHandler::GetMouseYaxis() {
	if (!m_curHidden) {
		return 0;
	}

	double yaxis;
	glfwGetCursorPos(m_win, NULL, &yaxis);

	return yaxis;
}

void EXP_InputHandler::ResetPointer() {
	if (m_curHidden) {
		glfwSetCursorPos(m_win, 0, 0);
	}
}