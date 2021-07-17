#pragma once
#include <EXP_Game.h>
#include <EXP_PointLight.h>
#include <EXP_DirLight.h>
#include <EXP_StaticMesh.h>
#include <EXP_Actor.h>

#include <vector>

struct EditorRegistry {
	std::vector<EXP_PointLight*> m_plights;
	std::vector<EXP_DirLight*> m_dlights;
	std::vector<EXP_StaticMesh*> m_meshes;
	std::vector<EXP_Actor*> m_actors;
};