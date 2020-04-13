#include "pch.h"
#include "RD_Mesh.h"

RD_Mesh::RD_Mesh(BD_MatDef material, vec3f position, vec3f rotation, vec3f scale) {
	inActor = false;
	m_actor_mat = glm::mat4(1.0f);
	m_nbr_indices = 0;

	VAO = 0;
	EBO = 0;
	VBO = 0;

	m_mat = new RD_SimpleMaterial(material);

	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

RD_Mesh::~RD_Mesh() {
	delete m_mat;
}

void RD_Mesh::loadMesh(std::string filepath) {
	BD_Reader* reader = new BD_Reader();

	reader->ClearAll();
	reader->ReadMSHFile(filepath);

	for (int i = 0; i < reader->GetVerticesCount(); i++) {
		m_vertices.push_back(reader->GetVertexByIndex(i));
	}

	for (int i = 0; i < reader->GetIndicesCount(); i++) {
		m_indices.push_back(reader->GetIndiceByIndex(i));
	}

	for (int i = 0; i < reader->GetNormalCount(); i++) {
		m_normals.push_back(reader->GetNormalByIndex(i));
	}

	for (int i = 0; i < reader->GetUVcoordCount(); i++) {
		m_uv.push_back(reader->GetUVcoordByIndex(i));
	}

	Bufferize();

	m_nbr_indices = (int) m_indices.size();

	m_vertices.clear();
	m_indices.clear();
	m_normals.clear();
	m_uv.clear();

	reader->ClearAll();
	delete reader;
}

void RD_Mesh::render(RD_ShaderLoader* shader, RenderMode rndrMode) {
	if (rndrMode == RenderMode::Filled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	shader->useShader();

	glm::mat4 mdl = glm::mat4(1.0f); //Declaring Model Matrix

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

	if (inActor) {
		mdl = translate * rotation * scale;
		mdl *= m_actor_mat;
	}
	else {
		mdl = translate * rotation * scale;
	}

	shader->SetMatrix("model", mdl);

	m_mat->BindMaterial(shader);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_nbr_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RD_Mesh::renderShadows(RD_ShaderLoader* shadowShader) {
	glm::mat4 mdl = glm::mat4(1.0f); //Declaring Model Matrix

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

	mdl = translate * scale * rotation;
	shadowShader->SetMatrix("model", mdl);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_nbr_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RD_Mesh::Bufferize() {

	for (int i = 0; i < m_vertices.size(); i++) {
		//Vertices

		MixVertNormUV.push_back(m_vertices[i].getX());
		MixVertNormUV.push_back(m_vertices[i].getY());
		MixVertNormUV.push_back(m_vertices[i].getZ());

		//Normals

		MixVertNormUV.push_back(m_normals[i].getX()); //Write
		MixVertNormUV.push_back(m_normals[i].getY()); //Write
		MixVertNormUV.push_back(m_normals[i].getZ()); //Write

		//UVs
		MixVertNormUV.push_back(m_uv[i].getX());
		MixVertNormUV.push_back(m_uv[i].getY());
	}

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, MixVertNormUV.size() * sizeof(double), &MixVertNormUV[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	//Vertices
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)(3 * sizeof(double)));
	glEnableVertexAttribArray(1);

	//UV Coords
	glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, 8 * sizeof(double), (void*)(6 * sizeof(double)));
	glEnableVertexAttribArray(2);
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

vec3f RD_Mesh::GetLocation() {
	return m_position;
}

void RD_Mesh::ApplyActorMatrix(glm::mat4 mat) {
	m_actor_mat = mat;
}

void RD_Mesh::SetActorMode(bool mode) {
	inActor = mode;
}