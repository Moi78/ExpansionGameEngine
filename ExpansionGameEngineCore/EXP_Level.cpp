#include "pch.h"
#include "EXP_Level.h"

EXP_Level::EXP_Level(EXP_MapLoader* map, bool OnStartEnable, bool OnTickEnable) {
	m_startEnabled = OnStartEnable;
	m_tickEnabled = OnTickEnable;

	m_isStart = true;

	m_map = map;
}

void EXP_Level::CallEvents() {
	if (m_startEnabled && m_isStart) {
		OnStart();
		m_isStart = false;
	}

	if (m_tickEnabled) {
		OnTick();
	}
}

EXP_MapLoader* EXP_Level::GetMap() {
	return m_map;
}
