#pragma once

#ifndef _EXP_HOTLOAD_H__
#define _EXP_HOTLOAD_H__


#ifdef _WIN32
    #define TYPEDEF(rVal, tName, ...) typedef rVal (__cdecl* tName)(__VA_ARGS__)
#else
    #define TYPEDEF(rVal, tName, ...) typedef rVal (*tName)(__VA_ARGS__)
#endif

#include "EXP_Game.h"
#include "EXP_MapLoader.h"

#ifdef _WIN32

#ifdef EXPANSIONGAMEENGINE_EXPORTS
#define EXPGE_API __declspec(dllexport)
#else
#define EXPGE_API __declspec(dllimport)
#endif

#define EXPGE_LEVEL_HEADER(obj) extern "C" __declspec(dllexport) obj* obj##Handler (EXP_Game*, EXP_MapLoader*);\
								extern "C" __declspec(dllexport) void obj##Remover (obj*)
#define EXPGE_LEVEL_CODE(obj) obj* obj##Handler(EXP_Game* game, EXP_MapLoader* lvl) { return new obj(game, lvl); }\
							  void obj##Remover(obj* objCode) { delete objCode; }

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

#else

#define EXPGE_LEVEL_HEADER(obj) extern "C" obj* obj##Handler (EXP_Game*, EXP_MapLoader*);\
								extern "C" void obj##Remover (obj*)
#define EXPGE_LEVEL_CODE(obj) obj* obj##Handler(EXP_Game* game, EXP_MapLoader* lvl) { return new obj(game, lvl); }\
							  void obj##Remover(obj* objCode) { delete objCode; }

#include <iostream>
#include <string>

#include <dlfcn.h>
#include <cerrno>

class EXP_HotLoad
{
public:
	EXP_HotLoad();
	~EXP_HotLoad();

	bool LoadLib(const char* libFile);

	template<typename T>
	T FetchLibHandler(const char* ObjectName) {
        std::cout << "Trying to get symbol " << ObjectName << "." << std::endl;
        
		void* Obj = dlsym(m_moduleHandle, ObjectName);
		if (Obj != nullptr) {
			T ObjectHandler = reinterpret_cast<T>(Obj);
            return ObjectHandler;
		}
		else {
            std::cerr << "ERROR: Cannot fetch object \"" << ObjectName << "\". ERROR: " << dlerror() << std::endl;
            
			return nullptr;
		}
	};

private:
	void* m_moduleHandle;
};

#endif //_WIN32

#endif
