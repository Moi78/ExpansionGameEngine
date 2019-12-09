#pragma once

#ifndef _BD_STRUCT_MAN_H__
#define _BD_STRUCT_MAN_H__

#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

#include <vector>
#include <string>

#include <vec3.h>

struct BULLDOZERFILEMANAGER_API BD_FBX_ImportOption {
	bool ImportMaterials;
	bool ImportTextures;
	bool ImportAnimations;
	bool ImportGlobalSettings;
};

struct BULLDOZERFILEMANAGER_API BD_MeshDef {
	std::vector<vec3d> vertices;
	std::vector<int> indices;
};

struct BULLDOZERFILEMANAGER_API BD_Resolution {
	int x;
	int y;
};

struct BULLDOZERFILEMANAGER_API BD_MatDef {
	vec3f Color;
	vec3f SpecularColor;
	float SpecularExp;
};

struct BULLDOZERFILEMANAGER_API BD_GameInfo {
	std::string RootGameContentFolder;
	std::string RootEngineContentFolder;
};

enum BULLDOZERFILEMANAGER_API RendererFeature {
	Specular, Lighting, Textures
};

enum class BULLDOZERFILEMANAGER_API RenderMode {
	Wireframe, Filled
};

#endif // _BD_STRUCT_MAN_H__