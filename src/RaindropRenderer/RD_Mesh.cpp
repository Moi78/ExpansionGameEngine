#include "pch.h"
#include "RD_Mesh.h"

RD_Mesh::RD_Mesh(RD_ShaderMaterial* shader, vec3f position, vec3f rotation, vec3f scale) {
	m_nbr_indices = 0;

	VAO = 0;
	EBO = 0;
	VBO = 0;

	assert(shader != nullptr && "Given material was nullptr");
	m_mat = shader;

	m_position = position;
	m_rotation = rotation;
	m_scale = scale;

	m_parent = glm::mat4(1.0);

	Update();
}

RD_Mesh::~RD_Mesh() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
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

void RD_Mesh::render(RD_Camera* cam, RenderMode rndrMode) {
	bool filled;
	if (rndrMode == RenderMode::Filled) {
		filled = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else {
		filled = false;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	cam->UseCamera(m_mat->GetShader());
	m_mat->GetShader()->SetMatrix("model", m_mdl);

	m_mat->BindMaterial();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_nbr_indices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	if (!filled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void RD_Mesh::renderShadows(RD_ShaderLoader* shadowShader) {
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

void RD_Mesh::addRotation(vec3f rotation, bool update) {
	m_rotation = m_rotation + rotation;
	if(update)
		Update();
}

void RD_Mesh::addTranslation(vec3f translation, bool update) {
	m_position = m_position + translation;
	if (update)
		Update();
}

void RD_Mesh::addScale(vec3f scale, bool update) {
	m_scale = m_scale + scale;
	if (update)
		Update();
}

void RD_Mesh::SetRotation(vec3f nRotation, bool update) {
	m_rotation = nRotation;
	if (update)
		Update();
}

void RD_Mesh::SetPosition(vec3f nPos, bool update) {
	m_position = nPos;
	if (update)
		Update();
}

void RD_Mesh::SetScale(vec3f nScale, bool update) {
	m_scale = nScale;
	if (update)
		Update();
}

vec3f RD_Mesh::GetLocation() {
	return m_position;
}

void RD_Mesh::Update() {
	//Local transform
	glm::mat4 translate = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rotation = glm::mat4(1.0f);

	//Position
	translate = glm::translate(translate, glm::vec3(m_position.getX(), m_position.getY(), m_position.getZ()));

	//Scale
	scale = glm::scale(scale, glm::vec3(m_scale.getX(), m_scale.getY(), m_scale.getZ()));

	//Rotation
	glm::quat rot(glm::radians(glm::vec3(m_rotation.getX(), m_rotation.getY(), m_rotation.getZ())));
	rotation = glm::toMat4(rot);

	m_mdl = translate * rotation * scale;
	m_mdl = m_parent * m_mdl;
}

RD_ShaderMaterial* RD_Mesh::GetMaterial() {
	return m_mat;
}

void RD_Mesh::UseMatrix(glm::mat4 mdl) {
	m_mdl = mdl;
}

void RD_Mesh::SetParentMatrix(glm::mat4 parent) {
	m_parent = parent;
	Update();
}