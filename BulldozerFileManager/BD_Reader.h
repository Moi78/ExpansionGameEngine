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

class BULLDOZERFILEMANAGER_API BD_Reader
{
public:
	BD_Reader();
	~BD_Reader();

	void ReadMSHFile(std::string);

	int GetIndiceByIndex(int);
	vec3d GetVertexByIndex(int);
	vec3d GetNormalByIndex(int);
	vec2d GetUVcoordByIndex(int);

	int GetIndicesCount();
	int GetVerticesCount();
	int GetNormalCount();
	int GetUVcoordCount();

	void ClearAll();

private:
	std::vector<int> mIndices;
	std::vector<vec3d> mVertices;
	std::vector<vec3d> mNormal;
	std::vector<vec2d> mUVcoord;

	int icount;
	int vcount;
	int ncount;
	int uvcount;
};

#endif // _BD_READER_H__