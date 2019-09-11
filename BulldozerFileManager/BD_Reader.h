#pragma once

#ifndef _BD_READER_H__
#define _BD_READER_H__

#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

#include "BulldozerFileManager.h"

#include <vec3.h>

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

	int GetIndicesCount();
	int GetVerticesCount();

	void ClearAll();

private:
	std::vector<int> mIndices;
	std::vector<vec3d> mVertices;

	int icount;
	int vcount;
};

#endif // _BD_READER_H__