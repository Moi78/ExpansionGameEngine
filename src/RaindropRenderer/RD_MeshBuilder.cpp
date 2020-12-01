#include "pch.h"
#include "RD_MeshBuilder.h"

RD_MeshBuilder::RD_MeshBuilder(RaindropRenderer* rndr, RD_ShaderMaterial* shader, vec3f pos, vec3f rot, vec3f scale) :
	RD_Mesh(rndr, shader, pos, rot, scale) {

}

RD_MeshBuilder::~RD_MeshBuilder() {}

void RD_MeshBuilder::LoadMeshBuffer(std::vector<vec3f> pos, std::vector<vec3f> norm, std::vector<vec2f> UV, std::vector<unsigned int> indices) {
	m_indices = indices;

	m_vertices = pos;
	m_normals = norm;
	m_uv = UV;

	Bufferize();

	m_nbr_indices = (int)m_indices.size();

	m_indices.clear();
	m_vertices.clear();
	m_normals.clear();
	m_uv.clear();
}

RD_Mesh* RD_MeshBuilder::GetMesh() {
	return reinterpret_cast<RD_Mesh*>(this);
}