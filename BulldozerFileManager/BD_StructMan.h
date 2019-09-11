#pragma once

#ifndef _BD_STRUCT_MAN_H__
#define _BD_STRUCT_MAN_H__

#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

#include <vector>

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

struct BULLDOZERFILEMANAGER_API BD_MatDef {
	vec3f Color;
};

#endif // _BD_STRUCT_MAN_H__