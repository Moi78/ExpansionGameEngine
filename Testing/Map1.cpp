#include "Map1.h"

Map1::Map1(EXP_Game* game, EXP_MapLoader* map) : EXP_Level(map, true, false) {
	m_donut = new Donut(game, vec3f(0.0f, 0.0f, 2.0f));
}

void Map1::OnStart() {
	std::cout << "OnStart() YAY" << std::endl;
}

void Map1::OnTick() {
	//GetMap()->GetStaticMeshByName("chose")->GetRawMeshData()->addRotation(vec3f(0.1f, 0.0f, 0.2f));

	std::cout << "Tick :/" << std::endl;
}