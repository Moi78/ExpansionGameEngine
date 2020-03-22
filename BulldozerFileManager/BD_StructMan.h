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
	unsigned int BaseColor;
	vec3f SpecularColor;
	float SpecularExp;
};

struct BULLDOZERFILEMANAGER_API BD_GameInfo {
	std::string RootGameContentFolder = "Engine";
	std::string RootEngineContentFolder = "Content";
	std::string GameName = "Default Game";
};

enum BULLDOZERFILEMANAGER_API RendererFeature {
	Specular, Lighting, Textures
};

enum class BULLDOZERFILEMANAGER_API RenderMode {
	Wireframe, Filled
};

enum class BULLDOZERFILEMANAGER_API BD_ComponentType {
	POINTLIGHT,
	STATIC_MESH,
	CAMERA
};

struct BULLDOZERFILEMANAGER_API BD_ComponentReference {
	BD_ComponentType type;
	std::string label;

	unsigned int actor_component_index;

	vec3f loc_pos;
	vec3f loc_rot;
	vec3f loc_scale;

	std::string args;

	bool isRoot;
};

#endif // _BD_STRUCT_MAN_H__