#pragma once

#ifdef _WIN32
	#ifdef EXPANSIONGAMEENGINECORE_EXPORTS
		#define EXPGE_API __declspec(dllexport)
	#else
		#define EXPGE_API __declspec(dllimport)
	#endif //EXPANSIONGAMEENGINECORE_EXPORTS
#else
	#define EXPGE_API
#endif //_WIN32

#include <string>

struct EXPGE_API EXP_Resolution {
	int x;
	int y;
};

struct EXPGE_API EXP_GameInfo {
	std::string RootGameContentFolder = "Engine";
	std::string RootEngineContentFolder = "Content";
	EXP_Resolution GameBaseResolution = { 1280, 720 };
	std::string GameLib = "Game.dll";

	std::string GameName = "Default Game";
	std::string StartupMap;
};