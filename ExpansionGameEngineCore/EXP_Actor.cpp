#include "pch.h"
#include "EXP_Actor.h"

#include "EXP_Game.h"

EXP_Actor::EXP_Actor(EXP_Game* game, vec3f pos, vec3f rot, vec3f scale, std::string actordef_file) {
	m_game = game;

	m_pos = pos;
	m_rot = rot;
	m_scale = scale;

	LoadDef(actordef_file);
}

EXP_Actor::~EXP_Actor() {

}

vec3f EXP_Actor::GetWorldPos() {
	return m_pos;
}

vec3f EXP_Actor::GetWorldRot() {
	return m_rot;
}

vec3f EXP_Actor::GetWorldScale() {
	return m_scale;
}

void EXP_Actor::LoadDef(std::string def) {
	BD_ActorReader* ar = new BD_ActorReader();
	ar->ReadActorDefinitionFile(m_game->GetGameInfo().RootGameContentFolder + def + ".actor");

	BD_ComponentReference buffer = {};
	pReader* args_r = new pReader();

	for (int i = 0; i < ar->GetComponentCount(); i++) {
		buffer = ar->GetComponentByIndex(i);

		args_r->ClearReader();

		std::cout << "Args : " << buffer.args << std::endl;


		//MESH


		if (buffer.type == BD_ComponentType::STATIC_MESH) {
			args_r->parse(buffer.args);

			std::string mshref = args_r->GetFromKey("ref");
			if (mshref == "") {
				std::cerr << "Missing mesh reference to " << buffer.label << std::endl;
				return;
			}

			BD_MatDef mat = m_game->GetDefaultMaterial();

			if (args_r->GetFromKey("mat_ref") != "") {
				mat = m_game->FetchMaterialFromFile(args_r->GetFromKey("mat_ref"));
			}

			EXP_StaticMesh* msh = new EXP_StaticMesh(m_game, mshref, mat, buffer.loc_pos, buffer.loc_rot, buffer.loc_scale);

			msh->GetRawMeshData()->ApplyActorMatrix(CalculateActorMatrix());
			msh->GetRawMeshData()->SetActorMode(true);

			buffer.actor_component_index = m_comp_meshes.size();

			m_comp_meshes.push_back(msh);
		}

		//POINTLIGHT

		else if (buffer.type == BD_ComponentType::POINTLIGHT) {
			args_r->parse(buffer.args);

			vec3f color(std::stof(args_r->GetFromKey("r")), std::stof(args_r->GetFromKey("g")), std::stof(args_r->GetFromKey("b")));

			EXP_PointLight* ptlight = new EXP_PointLight(m_game, buffer.loc_pos + GetWorldPos(), buffer.loc_rot, buffer.loc_scale, color, std::stoi(args_r->GetFromKey("intensity")));

			buffer.actor_component_index = m_compt_pt_light.size();

			m_compt_pt_light.push_back(ptlight);
		}
		else if (buffer.type == BD_ComponentType::CAMERA) {
			break;
		}

		m_refs.push_back(buffer);
	}
}

glm::mat4 EXP_Actor::CalculateActorMatrix() {
	glm::mat4 mat = glm::mat4(1.0f);

	//Position
	mat = glm::translate(mat, glm::vec3(m_pos.getX(), m_pos.getY(), m_pos.getZ()));

	//Scale
	mat = glm::scale(mat, glm::vec3(m_scale.getX(), m_scale.getY(), m_scale.getZ()));

	//Rotation
	mat = glm::rotate(mat, m_rot.getX(), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, m_rot.getY(), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, m_rot.getZ(), glm::vec3(0.0f, 0.0f, 1.0f));

	return mat;
}