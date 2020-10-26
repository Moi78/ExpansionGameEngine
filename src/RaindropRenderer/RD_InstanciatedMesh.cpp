#include "pch.h"
#include "RD_InstanciatedMesh.h"

RD_InstanciatedMesh::RD_InstanciatedMesh(RD_ShaderMaterial* mat) : RD_Mesh(mat, vec3f(), vec3f(), vec3f()) {
	m_parent = mat4f(1.0f);
}

RD_InstanciatedMesh::~RD_InstanciatedMesh() {

}

int RD_InstanciatedMesh::AppendInstance(RD_MeshInstance inst) {
	mat4f mdl(1.0f);
	//mdl.DBG_print_matrix();

	mat4f translate(1.0f);
	mat4f scale(1.0f);
	mat4f rotation(1.0f);

	//Position
	translate = TranslateMatrix(translate, inst.pos);

	//Scale
	scale = ScaleMatrix(scale, inst.scale);

	//Rotation
	/*glm::quat rot(glm::vec3(inst.rotation.getX(), inst.rotation.getY(), inst.rotation.getZ()));
	rotation = glm::toMat4(rot);*/
	rotation = RotateMatrix(rotation, inst.rotation);

	mdl = translate * rotation * scale;
	mdl = m_parent * mdl;

	//m_parent.DBG_print_matrix();

	m_mdls.push_back(std::pair<mat4f, RD_MeshInstance>(mdl, inst));

	return (int)m_mdls.size() - 1;
}

void RD_InstanciatedMesh::render(RD_Camera* cam, RenderMode rndrMode) {
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

	m_mat->BindMaterial();

	glBindVertexArray(VAO);

	for (auto &mdl : m_mdls) {
		m_mat->GetShader()->SetMatrix("model", mdl.first);

		glDrawElements(GL_TRIANGLES, m_nbr_indices, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);

	if (!filled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void RD_InstanciatedMesh::renderShadows(RD_ShaderLoader* shadowShader) {
	glBindVertexArray(VAO);

	for (auto &mdl : m_mdls) {
		shadowShader->SetMatrix("model", mdl.first);

		glDrawElements(GL_TRIANGLES, m_nbr_indices, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}

void RD_InstanciatedMesh::Update() {
	for (int i = 0; i < m_mdls.size(); i++) {
		UpdateIndex(i);
	}
}

void RD_InstanciatedMesh::UpdateIndex(int index) {
	mat4f translate(1.0f);
	mat4f scale(1.0f);
	mat4f rotation(1.0f);

	//Position
	translate = TranslateMatrix(translate, m_mdls[index].second.pos);

	//Scale
	scale = ScaleMatrix(scale, m_mdls[index].second.scale);

	//Rotation
	/*glm::quat rot(glm::vec3(m_mdls[index].second.rotation.getX(), m_mdls[index].second.rotation.getY(), m_mdls[index].second.rotation.getZ()));
	rotation = glm::toMat4(rot);*/
	rotation = RotateMatrix(rotation, m_mdls[index].second.rotation);

	m_mdls[index].first = translate * rotation * scale;
	m_mdls[index].first = m_parent * m_mdls[index].first;
}

void RD_InstanciatedMesh::addRotation(vec3f rotation, bool update) {
	for (auto &inst : m_mdls) {
		inst.second.rotation = inst.second.rotation + rotation;
	}

	if (update)
		Update();
}

void RD_InstanciatedMesh::addTranslation(vec3f translation, bool update) {
	for (auto &inst : m_mdls) {
		inst.second.pos = inst.second.pos + translation;
	}

	if (update)
		Update();
}

void RD_InstanciatedMesh::addScale(vec3f scale, bool update) {
	for (auto &inst : m_mdls) {
		inst.second.scale = inst.second.scale + scale;
	}

	if (update)
		Update();
}

void RD_InstanciatedMesh::SetInstPosition(vec3f nPos, int index, bool update) {
	if (index >= m_mdls.size()) {
		std::cerr << "ERROR: Index out of range, cannot get mesh inst." << std::endl;
		return;
	}

	m_mdls[index].second.pos = nPos;

	if(update)
		UpdateIndex(index);
}

void RD_InstanciatedMesh::SetInstRotation(vec3f nRot, int index, bool update) {
	if (index >= m_mdls.size()) {
		std::cerr << "ERROR: Index out of range, cannot get mesh inst." << std::endl;
		return;
	}

	m_mdls[index].second.rotation = nRot;

	if (update)
		UpdateIndex(index);
}

void RD_InstanciatedMesh::SetInstScale(vec3f nScale, int index, bool update) {
	if (index >= m_mdls.size()) {
		std::cerr << "ERROR: Index out of range, cannot get mesh inst." << std::endl;
		return;
	}

	m_mdls[index].second.scale = nScale;
	if (update)
		UpdateIndex(index);
}

void RD_InstanciatedMesh::AddInstTranslation(vec3f nPos, int index, bool update) {
	if (index >= m_mdls.size()) {
		std::cerr << "ERROR: Index out of range, cannot get mesh inst." << std::endl;
		return;
	}

	m_mdls[index].second.pos = m_mdls[index].second.pos + nPos;

	if (update)
		UpdateIndex(index);
}

void RD_InstanciatedMesh::AddInstRotation(vec3f nRot, int index, bool update) {
	if (index >= m_mdls.size()) {
		std::cerr << "ERROR: Index out of range, cannot get mesh inst." << std::endl;
		return;
	}

	m_mdls[index].second.rotation = m_mdls[index].second.rotation + nRot;

	if (update)
		UpdateIndex(index);
}

void RD_InstanciatedMesh::AddInstScale(vec3f nScale, int index, bool update) {
	if (index >= m_mdls.size()) {
		std::cerr << "ERROR: Index out of range, cannot get mesh inst." << std::endl;
		return;
	}

	m_mdls[index].second.scale = m_mdls[index].second.scale + nScale;

	if (update)
		UpdateIndex(index);
}