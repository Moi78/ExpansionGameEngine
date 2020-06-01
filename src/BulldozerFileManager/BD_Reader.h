#pragma once

#ifndef _BD_READER_H__
#define _BD_READER_H__

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

#include <vec3.h>
#include <vec2.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

class BULLDOZERFILEMANAGER_API BD_Reader
{
public:
	BD_Reader();
	~BD_Reader();

	void ReadMSHFile(std::string);

	int GetIndiceByIndex(int);
	vec3f GetVertexByIndex(int);
	vec3f GetNormalByIndex(int);
	vec2f GetUVcoordByIndex(int);

	int GetIndicesCount();
	int GetVerticesCount();
	int GetNormalCount();
	int GetUVcoordCount();

	void ClearAll();

private:
	std::vector<int> mIndices;
	std::vector<vec3f> mVertices;
	std::vector<vec3f> mNormal;
	std::vector<vec2f> mUVcoord;

	int icount;
	int vcount;
	int ncount;
	int uvcount;
};

#endif // _BD_READER_H__