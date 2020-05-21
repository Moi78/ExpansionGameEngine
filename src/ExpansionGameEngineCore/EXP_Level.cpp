#include "pch.h"
#include "EXP_Level.h"

EXP_Level::EXP_Level(bool OnStartEnable, bool OnTickEnable) {
	m_startEnabled = OnStartEnable;
	m_tickEnabled = OnTickEnable;

	m_isStart = true;
}

void EXP_Level::CallStart() {
	if (m_startEnabled && m_isStart) {
		OnStart();
		m_isStart = false;
	}
}

void EXP_Level::CallTick() {
	if (m_tickEnabled) {
		OnTick();
	}
}
