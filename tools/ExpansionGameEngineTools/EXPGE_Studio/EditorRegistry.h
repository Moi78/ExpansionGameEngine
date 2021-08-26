#pragma once
#include <EXP_Game.h>
#include <EXP_PointLight.h>
#include <EXP_DirLight.h>
#include <EXP_StaticMesh.h>
#include <EXP_Actor.h>

#include <vector>
#include <string>

struct MeshMeta {
	std::string material;
	std::string meshref;
};

struct EditorRegistry {
	std::string mapPath;
	std::string levelCodeObjectName;

	std::vector<EXP_PointLight*> m_plights;
	std::vector<EXP_DirLight*> m_dlights;
	std::vector<std::pair<EXP_StaticMesh*, MeshMeta>> m_meshes;
	std::vector<EXP_Actor*> m_actors;
};

void ClearRegistry(EditorRegistry* reg);