#pragma once

#ifndef _EXP_LEVEL_H__
#define _EXP_LEVEL_H__

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif
#else
	#define EXPGE_API
#endif

#include "EXP_MapLoader.h"

class EXPGE_API EXP_Level
{
public:
	EXP_Level(bool OnStartEnable = true, bool OnTickEnable = true);

	virtual void OnStart() = 0;
	virtual void OnTick() = 0;

	void CallStart();
	void CallTick();

private:
	bool m_startEnabled;
	bool m_tickEnabled;

	bool m_isStart;
};

#endif