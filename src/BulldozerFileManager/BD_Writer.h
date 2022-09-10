#pragma once

#ifndef _BD_WRITER_H__
#define _BD_WRITER_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <vec.h>

class BD_Writer
{
public:
	BD_Writer();
	~BD_Writer();

	void AppendVertex(vec3 vertex);
	void AppendNormal(vec3 normal);
	void AppendIndices(int indice);
	void AppendUVcoord(vec2 uv);
	void AppendVertexWeight(vec4 weight);

	void ToBinary(std::string filepath, std::string filename);

	void ClearWriter();

private:
	std::vector<vec3> m_vertices;
	std::vector<int> m_indices;
	std::vector<vec3> m_normals;
	std::vector<vec2> m_uv_coord;
	std::vector<vec4> m_vertex_weight;
};

#endif