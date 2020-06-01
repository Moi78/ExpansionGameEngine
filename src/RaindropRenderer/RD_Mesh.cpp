#include "pch.h"
#include "RD_Mesh.h"

RD_Mesh::RD_Mesh(BD_MatDef material, vec3f position, vec3f rotation, vec3f scale) {
	m_nbr_indices = 0;

	VAO = 0;
	EBO = 0;
	VBO = 0;

	m_mat = new RD_SimpleMaterial(material);

	m_position = vec3f(0.0f, 0.0f, 0.0f);
	m_rotation = vec3f(0.0f, 0.0f, 0.0f);
	m_scale = vec3f(1.0f, 1.0f, 1.0f);

	m_worldPos = position;
	m_worldRot = rotation;
	m_worldScale = scale;

	Update();
}

RD_Mesh::~RD_Mesh() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

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

	shader->SetMatrix("model", m_mdl);

	m_mat->BindMaterial(shader);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_nbr_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void RD_Mesh::renderShadows(RD_ShaderLoader* shadowShader) {
	glm::mat4 mdl = glm::mat4(1.0f); //Declaring Model Matrix

	shadowShader->SetMatrix("model", m_mdl);

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

	size_t elemSize = 8;

	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, MixVertNormUV.size() * sizeof(float), &MixVertNormUV[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	//Vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, elemSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, elemSize * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//UV Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, elemSize * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void RD_Mesh::addRotation(vec3f rotation) {
	m_rotation = m_rotation + rotation;
	Update();
}

void RD_Mesh::addTranslation(vec3f translation) {
	m_position = m_position + translation;
	Update();
}

void RD_Mesh::addScale(vec3f scale) {
	m_scale = m_scale + scale;
	Update();
}

void RD_Mesh::SetRotation(vec3f nRotation) {
	m_rotation = nRotation;
	Update();
}

void RD_Mesh::SetPosition(vec3f nPos) {
	m_position = nPos;
	Update();
}

void RD_Mesh::SetScale(vec3f nScale) {
	m_scale = nScale;
	Update();
}

vec3f RD_Mesh::GetLocation() {
	return m_position;
}

void RD_Mesh::SetWorldPosition(vec3f nPos) {
	m_worldPos = nPos;
	Update();
}

void RD_Mesh::SetWorldRotation(vec3f nRot) {
	m_worldRot = nRot;
	Update();
}

void RD_Mesh::SetWorldScale(vec3f nScale) {
	m_worldScale = nScale;
	Update();
}

void RD_Mesh::Update() {
	glm::mat4 mdl = glm::mat4(1.0f); //Declaring Model Matrix

//Local transform
	glm::mat4 translate = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rotation = glm::mat4(1.0f);

	//Position
	translate = glm::translate(translate, glm::vec3(m_position.getX(), m_position.getY(), m_position.getZ()));

	//Scale
	scale = glm::scale(scale, glm::vec3(m_scale.getX(), m_scale.getY(), m_scale.getZ()));

	//Rotation
	glm::quat rot(glm::vec3(m_rotation.getX(), m_rotation.getY(), m_rotation.getZ()));
	rotation = glm::toMat4(rot);

	mdl = translate * rotation * scale;

	//World Transform
	glm::mat4 wmdl = glm::mat4(1.0f);
	glm::mat4 wtranslate = glm::mat4(1.0f);
	glm::mat4 wscale = glm::mat4(1.0f);
	glm::mat4 wrotation = glm::mat4(1.0f);

	//Rotation
	glm::quat wrot(glm::vec3(m_worldRot.getX(), m_worldRot.getY(), m_worldRot.getZ()));
	wrotation *= glm::toMat4(wrot);

	//Scale
	wscale = glm::scale(wscale, glm::vec3(m_worldScale.getX(), m_worldScale.getY(), m_worldScale.getZ()));

	//Position
	wtranslate = glm::translate(wtranslate, glm::vec3(m_worldPos.getX(), m_worldPos.getY(), m_worldPos.getZ()));

	//World model matrix
	wmdl = wtranslate * wrotation * wscale;

	//Final mat
	m_mdl = wmdl * mdl;
}