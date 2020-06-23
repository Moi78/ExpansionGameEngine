#include "pch.h"
#include "map2.h"

EXPGE_LEVEL_CODE(map2);

map2::map2(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {

}

map2::~map2() {

}

void map2::OnStart() {
	std::cout << "Map 2 STARTED !" << std::endl;
}

void map2::OnTick() {
	std::cout << "Map 2 TICKED" << std::endl;
}