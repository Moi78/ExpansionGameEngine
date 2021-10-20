#pragma once

#ifndef _BD_WRITER_H__
#define _BD_WRITER_H__

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

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <vec3.h>
#include <vec2.h>
#include <vec4.h>

#include "BulldozerFileManager.h"

class BULLDOZERFILEMANAGER_API BD_Writer
{
public:
	BD_Writer();
	~BD_Writer();

	void AppendVertex(vec3f vertex);
	void AppendNormal(vec3f normal);
	void AppendIndices(int indice);
	void AppendUVcoord(vec2f uv);
	void AppendVertexWeight(vec4f weight);

	void ToBinary(std::string filepath, std::string filename);

	void ClearWriter();

private:
	std::vector<vec3f> m_vertices;
	std::vector<int> m_indices;
	std::vector<vec3f> m_normals;
	std::vector<vec2f> m_uv_coord;
	std::vector<vec4f> m_vertex_weight;
};

#endif