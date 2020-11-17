#include "pch.h"
#include "RD_Mesh.h"

RD_Mesh::RD_Mesh(RaindropRenderer* rndr, RD_ShaderMaterial* shader, vec3f position, vec3f rotation, vec3f scale) : m_mdl(1.0f), m_parent(1.0f) {
	m_nbr_indices = 0;

	assert(shader != nullptr && "Given material was nullptr");
	m_mat = shader;

	m_position = position;
	m_rotation = rotation;
	m_scale = scale;

	m_rndr = rndr;
	m_buffer = m_rndr->GetRenderingAPI()->CreateVertexElemBuffer();

	m_shadowCaster = true;

	Update();
}

RD_Mesh::~RD_Mesh() {
	
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

void RD_Mesh::render(RD_Camera* cam) {

	cam->UseCamera(m_mat->GetShader());
	m_mat->GetShader()->SetMatrix("model", m_mdl);

	m_mat->BindMaterial();

	m_buffer->BindBuffer();
	m_rndr->GetRenderingAPI()->Draw(m_buffer);
	m_buffer->UnbindBuffer();
}

void RD_Mesh::renderShadows(RD_ShaderLoader* shadowShader) {
	if (!m_shadowCaster)
		return;

	shadowShader->SetMatrix("model", m_mdl);

	m_buffer->BindBuffer();
	m_rndr->GetRenderingAPI()->Draw(m_buffer);
	m_buffer->UnbindBuffer();
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

	m_buffer->CreateBuffer();

	m_buffer->FillBufferData(&MixVertNormUV[0], MixVertNormUV.size(), &m_indices[0], m_indices.size());
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
	/*glm::mat4 translate = glm::mat4(1.0f);
	glm::mat4 scale = glm::mat4(1.0f);
	glm::mat4 rotation = glm::mat4(1.0f);*/
	mat4f translate(1.0f);
	mat4f scale(1.0f);
	mat4f rotation(1.0f);

	//Position
	translate = TranslateMatrix(translate, m_position);

	//Scale
	scale = ScaleMatrix(scale, m_scale);

	//Rotation
	/*glm::quat rot(glm::radians(glm::vec3(m_rotation.getX(), m_rotation.getY(), m_rotation.getZ())));
	rotation = glm::toMat4(rot);*/
	rotation = RotateMatrix(rotation, m_rotation);

	m_mdl = translate * rotation * scale;
	m_mdl = m_parent * m_mdl;
}

RD_ShaderMaterial* RD_Mesh::GetMaterial() {
	return m_mat;
}

void RD_Mesh::UseMatrix(mat4f mdl) {
	m_mdl = mdl;
}

void RD_Mesh::SetParentMatrix(mat4f parent) {
	m_parent = parent;
	Update();
}

void RD_Mesh::SetShadowCasting(bool scasting) {
	m_shadowCaster = scasting;
}