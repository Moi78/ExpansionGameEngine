#pragma once

#ifndef _BD_IMPORT_H__
#define _BD_IMPORT_H__

#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

//Standard
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

//Bulldozer
#include "BulldozerFileManager.h"
#include "BD_StructMan.h"

//ExpansionMath
#include "vec3.h"

class BULLDOZERFILEMANAGER_API BD_Import
{
public:
	BD_Import();
	~BD_Import();

	void ImportFBX(std::string, BD_FBX_ImportOption);
	void ImportImageFile(std::string filepath);

	int GetIndiceInMemByIndex(int index);
	vec3d GetVertexInMemByIndex(int index);
	vec3d GetNormalInMemByIndex(int index);

	int GetVertexInMemCount();
	int GetIndicesInMemCount();
	int GetNormalInMemCount();

	void ToBinary(std::string, std::string);

	void AppendIndice(int);
	void AppendVertex(vec3d);
	void AppendNormal(vec3d);

private:
	void ClearAll();

	std::string m_filename;

	std::vector<vec3d> mVerticesInMem;
	std::vector<int> mIndicesInMem;
	std::vector<vec3d> mNormalsInMem;
};
#endif