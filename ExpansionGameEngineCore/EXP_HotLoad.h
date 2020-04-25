#pragma once

#ifndef _EXP_HOTLOAD_H__
#define _EXP_HOTLOAD_H__

#define TYPEDEF(rVal, tName, ...) typedef rVal (__cdecl* tName)(__VA_ARGS__)

#ifdef _WIN32

#ifdef EXPANSIONGAMEENGINE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#include "EXP_Game.h"
#include "EXP_MapLoader.h"

#define EXPGE_LEVEL_HEADER(obj) extern "C" __declspec(dllexport) obj* obj##Handler (EXP_Game*, EXP_MapLoader*)
#define EXPGE_LEVEL_CODE(obj) obj* obj##Handler(EXP_Game* game, EXP_MapLoader* lvl) { return new obj(game, lvl); }

#include <iostream>

#include <Windows.h>

class EXPGE_API EXP_HotLoad
{
public:
	EXP_HotLoad();
	~EXP_HotLoad();

	bool LoadLib(LPCWSTR libFile);

	template<typename T>
	T FetchLibHandler(const char* ObjectName) {
		T ObjectHandler = (T)GetProcAddress(m_moduleHandle, ObjectName);
		if (ObjectHandler != NULL) {
			return ObjectHandler;
		}
		else {
			std::cout << "ERROR: Cannot fetch object \"" << ObjectName << "\". ERROR CODE:" << GetLastError() << std::endl;
			return NULL;
		}
	};

private:
	HINSTANCE m_moduleHandle;
};

#endif //_WIN32

#endif