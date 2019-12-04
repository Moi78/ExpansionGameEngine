#include "pch.h"
#include "RD_Mesh.h"

RD_Mesh::RD_Mesh(RD_ShaderLoader* shader, BD_MatDef material, vec3f position, vec3f rotation, vec3f scale) {
	VAO = 0;
	EBO = 0;
	VBO = 0;

	m_mat = new RD_SimpleMaterial(shader, material.Color, material.SpecularColor, material.SpecularExp);

	m_shader = shader;

	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

RD_Mesh::~RD_Mesh() {

}

void RD_Mesh::loadMesh(std::string filepath) {
	BD_Reader* reader = new BD_Reader();

	reader->ClearAll();
	reader->ReadMSHFile(filepath);

	for (int i = 0; i < reader->GetVerticesCount(); i++) {
		RAWvertices.push_back(reader->GetVertexByIndex(i).getX());
		RAWvertices.push_back(reader->GetVertexByIndex(i).getY());
		RAWvertices.push_back(reader->GetVertexByIndex(i).getZ());
	}

	for (int i = 0; i < reader->GetIndicesCount(); i++) {
		RAWindices.push_back(reader->GetIndiceByIndex(i));
	}

	for (int i = 0; i < reader->GetNormalCount(); i++) {
		RAWnormals.push_back(reader->GetNormalByIndex(i).getX());
		RAWnormals.push_back(reader->GetNormalByIndex(i).getY());
		RAWnormals.push_back(reader->GetNormalByIndex(i).getZ());
	}

	Bufferize();

	delete reader;
}

void RD_Mesh::render() {
	//m_shader->useShader();
	m_mat->BindMaterial();

	glm::mat4 mdl = glm::mat4(1.0f);

	glm::mat4 translate = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rotation = glm::mat4(1.0f);

	//Position
	translate = glm::translate(translate, glm::vec3(m_position.getX(), m_position.getY(), m_position.getZ()));

	//Scale
	scale = glm::scale(scale, glm::vec3(m_scale.getX(), m_scale.getY(), m_scale.getZ()));

	//Rotation
	rotation = glm::rotate(rotation, glm::radians(m_rotation.getX()), glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(m_rotation.getY()), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(m_rotation.getZ()), glm::vec3(0.0f, 0.0f, 1.0f));

	mdl = translate * rotation * scale;

	m_shader->SetMatrix("model", mdl);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, RAWindices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RD_Mesh::Bufferize() {
	std::vector<float> MIXvertNorm;

	int i = 0;
	while (i < RAWvertices.size()) {
		//Vertices

		MIXvertNorm.push_back(RAWvertices[i]); //Write

		i++; //Increment

		MIXvertNorm.push_back(RAWvertices[i]); //Same Pattern

		i++;

		MIXvertNorm.push_back(RAWvertices[i]); //Same Pattern

		i++;

		i -= 3; //Resetting i

		//Normals

		MIXvertNorm.push_back(RAWnormals[i]); //Write

		i++; //Increment

		MIXvertNorm.push_back(RAWnormals[i]); //Same Pattern

		i++;

		MIXvertNorm.push_back(RAWnormals[i]); //Same Pattern

		i++;
	}

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, MIXvertNorm.size() * sizeof(float), &MIXvertNorm[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, RAWindices.size() * sizeof(unsigned int), &RAWindices[0], GL_STATIC_DRAW);

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void RD_Mesh::addRotation(vec3f rotation) {
	m_rotation = m_rotation + rotation;
}

void RD_Mesh::addTranslation(vec3f translation) {
	m_position = m_position + translation;
}

void RD_Mesh::addScale(vec3f scale) {
	m_scale = m_scale + scale;
}

void RD_Mesh::SetRotation(vec3f nRotation) {
	m_rotation = nRotation;
}

void RD_Mesh::SetPosition(vec3f nPos) {
	m_position = nPos;
}

void RD_Mesh::SetScale(vec3f nScale) {
	m_scale = nScale;
}

void RD_Mesh::UpdateMaterial(BD_MatDef* mat) {
	m_mat->SetBaseColor(mat->Color);
}