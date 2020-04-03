#pragma once

#ifndef _BD_MAT_RW_H__
#define _BD_MAT_RW_H__

#ifdef _WIN32
#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

#include <Windows.h>

#else

#define BULLDOZERFILEMANAGER_API

#endif //_WIN32
#include "BulldozerFileManager.h"
#include "BD_StructMan.h"

#include <iostream>
#include <string>
#include <fstream>

//Write

class BULLDOZERFILEMANAGER_API BD_MatWrite
{
public:
	BD_MatWrite();
	~BD_MatWrite();
	
	bool WriteMaterialToFile(BD_MatDef material, std::string filepath);
};

//Read

class BULLDOZERFILEMANAGER_API BD_MatRead {
public:
	BD_MatRead();
	~BD_MatRead();

	BD_MatDef ReadMaterialFromFile(std::string filepath);
};

#endif //_BD_MAT_RW_H__