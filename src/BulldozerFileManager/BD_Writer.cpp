#include "BD_Writer.h"

BD_Writer::BD_Writer() {

}

BD_Writer::~BD_Writer() {
	ClearWriter();
}

void BD_Writer::AppendVertex(vec3 vertex) {
	m_vertices.push_back(vertex);
}

void BD_Writer::AppendIndices(int indice) {
	m_indices.push_back(indice);
}

void BD_Writer::AppendNormal(vec3 normal) {
	m_normals.push_back(normal);
}

void BD_Writer::AppendUVcoord(vec2 uv) {
	m_uv_coord.push_back(uv);
}

void BD_Writer::AppendVertexWeight(vec4 weight) {
	m_vertex_weight.push_back(weight);
}

void BD_Writer::ToBinary(std::string filepath, std::string filename) {
	std::string ddir = filepath + filename + ".msh";

	std::ofstream bFile;
	bFile.open(ddir.c_str(), std::ios::binary);

	if (!bFile) {
		std::cerr << "Cannot create bin file : " << ddir;

		return;
	}

	uint32_t nbrVertices = m_vertices.size();
    uint32_t nbrIndice = m_indices.size();
    uint32_t nbrNormal = m_normals.size();
    uint32_t nbrUVcoord = m_uv_coord.size();
    uint32_t nbrVertStrength = m_vertex_weight.size();

	//Vertices
	bFile.write(reinterpret_cast<const char*>(&nbrVertices), sizeof(uint32_t));

	for (int i = 0; i < nbrVertices; i++) {
		bFile.write(reinterpret_cast<const char*>(&m_vertices[i]), sizeof(vec3));
	}

	//Indices
	bFile.write(reinterpret_cast<const char*>(&nbrIndice), sizeof(int));

	for (int i = 0; i < nbrIndice; i++) {
		bFile.write(reinterpret_cast<const char*>(&m_indices[i]), sizeof(int));
	}

	//Normals
	bFile.write(reinterpret_cast<const char*>(&nbrNormal), sizeof(int));

	for (int i = 0; i < nbrNormal; i++) {
		bFile.write(reinterpret_cast<const char*>(&m_normals[i]), sizeof(vec3));
	}

	//UV Coords
	bFile.write(reinterpret_cast<const char*>(&nbrUVcoord), sizeof(int));

	for (int i = 0; i < nbrUVcoord; i++) {
		bFile.write(reinterpret_cast<const char*>(&m_uv_coord[i]), sizeof(vec2));
	}

	//Vertex Strength
	bFile.write(reinterpret_cast<const char*>(&nbrVertStrength), sizeof(int));

	for (int i = 0; i < nbrVertStrength; i++) {
		bFile.write(reinterpret_cast<const char*>(&m_vertex_weight[i]), sizeof(vec4));
	}

	bFile.close();
}

void BD_Writer::ClearWriter() {
	m_indices.clear();
	m_vertices.clear();
	m_normals.clear();
	m_uv_coord.clear();
}