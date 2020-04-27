#pragma once

#ifndef _BD_STRUCT_MAN_H__
#define _BD_STRUCT_MAN_H__

#ifdef _WIN32
#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

#include <Windows.h>

#else

#define BULLDOZERFILEMANAGER_API

#endif //_WIN32

#include <vector>
#include <string>

#include <vec3.h>

struct BULLDOZERFILEMANAGER_API BD_Resolution {
	int x;
	int y;
};

struct BULLDOZERFILEMANAGER_API BD_MatDef {
	unsigned int BaseColor;

	unsigned int NormalMap;
	bool NormalEnabled = false;

	unsigned int Specular;
	float Shininess;
};

struct BULLDOZERFILEMANAGER_API BD_GameInfo {
	std::string RootGameContentFolder = "Engine";
	std::string RootEngineContentFolder = "Content";
	BD_Resolution GameBaseResolution = {1280, 720};
	std::string GameLib = "Game.dll";

	std::string GameName = "Default Game";
	std::string StartupMap;
};

enum BULLDOZERFILEMANAGER_API RendererFeature {
	Specular, Lighting, Ambient
};

enum class BULLDOZERFILEMANAGER_API RenderMode {
	Wireframe, Filled
};

struct BULLDOZERFILEMANAGER_API Gbuff {
	unsigned int gBuff;
	unsigned int gRBO;

	unsigned int gPos;
	unsigned int gAlbedo;
	unsigned int gSpec;
	unsigned int gShadows;

	unsigned int gNorm;

	unsigned int gAttachement[5];
};

#endif // _BD_STRUCT_MAN_H__