#pragma once

#ifndef _MAP1_H__
#define _MAP1_H__

#include <iostream>

#include <EXP_Game.h>
#include <EXP_Level.h>
#include <EXP_MapLoader.h>

#include "Donut.h"

class Map1 : public EXP_Level
{
public:
	Map1(EXP_Game* game, EXP_MapLoader*);

	virtual void OnStart() override;
	virtual void OnTick() override;

private:
	Donut* m_donut;
};

#endif