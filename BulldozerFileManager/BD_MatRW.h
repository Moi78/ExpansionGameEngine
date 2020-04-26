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
    #include <json/json.h>

#else

    #define BULLDOZERFILEMANAGER_API

    #include <jsoncpp/json/json.h>

#endif //_WIN32

#include "BulldozerFileManager.h"
#include "BD_StructMan.h"

#include <iostream>
#include <string>
#include <fstream>

//Write

struct BD_WriteMdef {
	std::string BaseColorRef = "";
	vec3f BaseColor;

	std::string SpecularColorRef = "";
	vec3f SpecularColor;

	float Shininess;
};

class BULLDOZERFILEMANAGER_API BD_MatWrite
{
public:
	BD_MatWrite();
	~BD_MatWrite();
	
	bool WriteMaterialToFile(BD_WriteMdef material, std::string filepath);
};

//Read

class BULLDOZERFILEMANAGER_API BD_MatRead {
public:
	BD_MatRead();
	~BD_MatRead();

	BD_WriteMdef ReadMaterialFromFile(std::string filepath);
};

#endif //_BD_MAT_RW_H__
