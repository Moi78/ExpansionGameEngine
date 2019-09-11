#include "pch.h"
#include "RD_Mesh.h"

RD_Mesh::RD_Mesh(RD_ShaderLoader* shader, BD_MatDef material, vec3f position, vec3f rotation, vec3f scale) {
	VAO = 0;
	EBO = 0;
	VBO = 0;

	m_mat = new RD_SimpleMaterial(shader, vec3f(material.Color.getX(), material.Color.getY(), material.Color.getZ()));

	m_shader = shader;

	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

RD_Mesh::~RD_Mesh() {

}

void RD_Mesh::loadMesh(std::string filepath) {
	BD_Reader* reader = new BD_Reader();

	reader->ReadMSHFile(filepath);

	for (int i = 0; i < reader->GetIndicesCount(); i++) {
		RAWindices.push_back(reader->GetIndiceByIndex(i));
	}

	for (int i = 0; i < reader->GetVerticesCount(); i++) {
		RAWvertices.push_back(reader->GetVertexByIndex(i).getX());
		RAWvertices.push_back(reader->GetVertexByIndex(i).getY());
		RAWvertices.push_back(reader->GetVertexByIndex(i).getZ());
	}

	Bufferize();

	delete reader;
}

void RD_Mesh::render() {
	m_mat->BindMaterial();

	glm::mat4 mdl = glm::mat4(1.0f);
	//Scale
	mdl = glm::scale(mdl, glm::vec3(m_scale.getX(), m_scale.getY(), m_scale.getZ()));

	//Rotation
	mdl = glm::rotate(mdl, glm::radians(m_rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
	mdl = glm::rotate(mdl, glm::radians(m_rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
	mdl = glm::rotate(mdl, glm::radians(m_rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));

	//Position
	mdl = glm::translate(mdl, glm::vec3(m_position.getX(), m_position.getY(), m_position.getZ()));

	m_shader->SetMatrix("model", mdl);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, RAWindices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RD_Mesh::Bufferize() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, RAWvertices.size() * sizeof(float), &RAWvertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, RAWindices.size() * sizeof(unsigned int), &RAWindices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void RD_Mesh::addRotation(vec3f rotation) {
	m_rotation = m_rotation + rotation;
}