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

#include <iostream>
#include <string>
#include <fstream>

#include <vec3.h>

//Write

struct BD_WriteMdef {
	std::string BaseColorRef = "";
	vec3f BaseColor;

	std::string SpecularColorRef = "";
	vec3f SpecularColor;

	std::string NormalMap = "";

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

//Custom Shader Read
class BULLDOZERFILEMANAGER_API BD_MatCustomShaderRead {
public:
	BD_MatCustomShaderRead(std::string matFile);
	~BD_MatCustomShaderRead();

	void CloseFile();

	std::string GetShaderCode(bool legacyCode);
	int GetTextureCount();
	std::string GetTextureParamName(int index);
	std::string GetTexturePath(int index);

private:
	std::ifstream m_file;
	Json::Value m_root;
};

class BULLDOZERFILEMANAGER_API BD_MatCustomShaderWrite {
public:
	BD_MatCustomShaderWrite();
	~BD_MatCustomShaderWrite();

	void SetShaderCode(std::string code);
	void SetShaderCodeOldGL(std::string code);
	void AddTextureRef(std::string ref, std::string param);

	bool WriteMaterialFile(std::string path);

private:
	std::vector<std::pair<std::string, std::string>> m_texs;

	std::string m_shader_code;
	std::string m_shader_code_leg;
};

#endif //_BD_MAT_RW_H__
