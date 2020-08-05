#include "pch.h"
#include "cave.h"

EXPGE_LEVEL_CODE(cave);

cave::cave(EXP_Game* game, EXP_MapLoader* mloader) : EXP_Level(true, true) {
	m_character = new Character(game);
}

cave::~cave() {

}

void cave::OnStart() {

}

void cave::OnTick() {

}