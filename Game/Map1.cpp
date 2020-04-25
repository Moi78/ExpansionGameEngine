#include "pch.h"
#include "Map1.h"

Map1::Map1(EXP_Game* game, EXP_MapLoader* mload) : EXP_Level(mload, true, true) {
	std::cout << "Hello map" << std::endl;

	m_cam = new EXP_Camera(game, vec3f(-4.0f, 0.0, 2.0f), vec3f(), vec3f(), vec3f(90.0f), 60.0f);
	m_cam->Use();
}

Map1::~Map1() {

}

void Map1::OnStart() {

}

void Map1::OnTick() {

}

EXPGE_LEVEL_CODE(Map1);