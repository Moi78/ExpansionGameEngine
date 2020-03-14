#pragma once

#ifndef _BD_WRITER_H__
#define _BD_WRITER_H__

#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <vec3.h>
#include <vec2.h>

#include "BulldozerFileManager.h"

class BULLDOZERFILEMANAGER_API BD_Writer
{
public:
	BD_Writer();
	~BD_Writer();

	void AppendVertex(vec3d vertex);
	void AppendNormal(vec3d normal);
	void AppendIndices(int indice);
	void AppendUVcoord(vec2d uv);

	void ToBinary(std::string filepath, std::string filename);

	void ClearWriter();

private:
	std::vector<vec3d> m_vertices;
	std::vector<int> m_indices;
	std::vector<vec3d> m_normals;
	std::vector<vec2d> m_uv_coord;
};

#endif