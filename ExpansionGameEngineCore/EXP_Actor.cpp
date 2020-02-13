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

			EXP_StaticMesh* msh = new EXP_StaticMesh(m_game, mshref, mat, buffer.loc_pos + GetWorldPos(), buffer.loc_rot + GetWorldRot(), buffer.loc_scale * GetWorldScale());

			buffer.actor_component_index = m_comp_meshes.size();

			m_comp_meshes.push_back(msh);
		}
		else if (buffer.type == BD_ComponentType::POINTLIGHT) {
			args_r->parse(buffer.args);

			vec3f color(std::stof(args_r->GetFromKey("r")), std::stof(args_r->GetFromKey("g")), std::stof(args_r->GetFromKey("b")));

			EXP_PointLight* ptlight = new EXP_PointLight(m_game, buffer.loc_pos + GetWorldPos(), buffer.loc_rot + GetWorldRot(), buffer.loc_scale * GetWorldScale(), color, std::stoi(args_r->GetFromKey("intensity")));

			buffer.actor_component_index = m_compt_pt_light.size();

			m_compt_pt_light.push_back(ptlight);
		}
		else if (buffer.type == BD_ComponentType::CAMERA) {
			break;
		}

		m_refs.push_back(buffer);
	}
}