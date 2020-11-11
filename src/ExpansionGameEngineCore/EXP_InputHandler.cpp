#include "pch.h"
#include "EXP_InputHandler.h"

EXP_InputHandler::EXP_InputHandler(RD_WindowingSystem* win) {
	m_win = win;
	m_curHidden = false;
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

void EXP_InputHandler::RegisterMouseButtonCallback(EXP_MouseButtonCallback* cllbck) {
	m_mouse_callbacks.push_back(cllbck);
}

void EXP_InputHandler::UnregisterMouseButtonCallback(EXP_MouseButtonCallback* cllbck) {
	int index = GetElemIndex<EXP_MouseButtonCallback*>(m_mouse_callbacks, cllbck);

	if (index != -1) {
		m_mouse_callbacks.erase(m_mouse_callbacks.begin() + index);
	}
	else {
		std::cerr << "ERROR: Element does not exists" << std::endl;
	}
}

void EXP_InputHandler::CaptureCursor(bool state) {
	m_win->CaptureCursor(state);

	m_curHidden = state;
}

void EXP_InputHandler::UpdateKeyboardInput() {
	for (auto cllbck : m_kb_callbacks) {
		cllbck->UpdateCallback();
	}
}

void EXP_InputHandler::UpdateMouseInput() {
	for (auto cllbck : m_mouse_callbacks) {
		cllbck->UpdateCallback();
	}
}

double EXP_InputHandler::GetMouseXaxis() {
	if (!m_curHidden) {
		return 0;
	}

	return m_win->GetCursorPosX();
}

double EXP_InputHandler::GetMouseYaxis() {
	if (!m_curHidden) {
		return 0;
	}

	return m_win->GetCursorPosY();
}

double EXP_InputHandler::GetAbsoluteMousePosX() {
	if (m_curHidden) {
		return 0;
	}

	return m_win->GetCursorPosX();
}

double EXP_InputHandler::GetAbsoluteMousePosY() {
	if (m_curHidden) {
		return 0;
	}

	return m_win->GetCursorPosY();
}

double EXP_InputHandler::GetGUI_SpaceMousePosX() {
	int w = m_win->GetWidth();

	double x = m_win->GetCursorPosX();

	return (x * 1280) / w;
}

double EXP_InputHandler::GetGUI_SpaceMousePosY() {
	int w = m_win->GetWidth();
	int h = m_win->GetHeight();

	double y = m_win->GetCursorPosY();

	float rh = 1280 / ((float)w / (float)h);
	return (y * rh) / h;
}

void EXP_InputHandler::ResetPointer() {
	if (m_curHidden) {
		m_win->SetCursorPos(0, 0);
	}
}

void EXP_InputHandler::UnregisterAllCallbacks() {
	for (auto elem : m_kb_callbacks) {
		std::cout << "Unregistering keyboard callback." << std::endl;
		if (elem)
			delete elem;
	}
	m_kb_callbacks.clear();

	for (auto elem : m_mouse_callbacks) {
		std::cout << "Unregistering mouse callback." << std::endl;
		if (elem)
			delete elem;
	}
	m_mouse_callbacks.clear();
}

bool EXP_InputHandler::GetMouseButton(int button) {
	return m_win->GetMouseButton(button);
}

bool EXP_InputHandler::GetKey(int key) {
	return m_win->GetKeyPress(key);
}