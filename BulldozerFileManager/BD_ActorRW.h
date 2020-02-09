#pragma once

#ifndef _BD_ACTOR_RW_H__
#define _BD_ACTOR_RW_H__

#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

#include "BD_StructMan.h"
#include "BulldozerFileManager.h"

#include <pReader.h>

//Actor Write

class BULLDOZERFILEMANAGER_API BD_ActorWriter
{
public:
	BD_ActorWriter();
	~BD_ActorWriter();

	void SetRootComponent(std::string comp_label, BD_ComponentType type, std::string args);

	void AddComponent(std::string comp_label, vec3f pos, vec3f rot, vec3f scale, BD_ComponentType type, std::string args);

	void WriteActorFile(std::string actor_name);

private:
	std::vector<BD_ComponentReference> m_comps;
};

//Actor Read

class BULLDOZERFILEMANAGER_API BD_ActorReader {
public:
	BD_ActorReader();
	~BD_ActorReader();

	void ReadActorDefinitionFile(std::string filepath);

	int GetComponentCount();

	BD_ComponentReference GetRootComponent();
	BD_ComponentReference GetComponentByIndex(int index);

private:
	std::vector<BD_ComponentReference> m_comps;
};

#endif //_BD_ACTOR_RW_H__