#include "pch.h"
#include "EXP_MapLoader.h"

EXP_MapLoader::EXP_MapLoader(EXP_Game* game, std::string Mapfile) {
	m_map = std::ifstream(Mapfile);
	m_game = game;
}

EXP_MapLoader::~EXP_MapLoader() {

}

bool EXP_MapLoader::LoadMap() {
	Json::Value root;
	JSONCPP_STRING errs;

	Json::CharReaderBuilder builder;
	builder["collectComment"] = false;

	if (!Json::parseFromStream(builder, m_map, &root, &errs)) {
		std::cerr << "Cannot load map file. " << errs << std::endl;
		return false;
	}

	int nodeCount = root.get("nodeCount", "0").asInt();
	for (int i = 0; i < nodeCount; i++) {

		Json::Value node = root[std::to_string(i)];
		std::string type = node.get("type", "mesh").asString();

		//Creating map by specified type
		//Actors must be spawned from a EXP_Level child class

		if (type == "mesh") { //Mesh type
			std::string ref = node.get("ref", "/").asString();

			Json::Value pos = node["pos"];
			vec3f mpos(pos[0].asFloat(), pos[1].asFloat(), pos[2].asFloat());

			Json::Value rot = node["rot"];
			vec3f mrot(rot[0].asFloat(), rot[1].asFloat(), rot[2].asFloat());

			Json::Value scale = node["scale"];
			vec3f mscale(scale[0].asFloat(), scale[1].asFloat(), scale[2].asFloat());

			EXP_StaticMesh* mesh = new EXP_StaticMesh(m_game, ref, m_game->GetDefaultMaterial(), mpos, mrot, mscale);
			mesh->SetNameTag(node["nameTag"].asString());
			m_meshes.push_back(mesh);

		} else if (type == "plight") { //PointLight type
			Json::Value pos = node["pos"];
			vec3f mpos(pos[0].asFloat(), pos[1].asFloat(), pos[2].asFloat());

			Json::Value color = node["color"];
			vec3f mcolor(color[0].asFloat(), color[1].asFloat(), color[2].asFloat());

			EXP_PointLight* plight = new EXP_PointLight(m_game, mpos, mcolor, node.get("brightness", "1.0").asFloat());
			plight->SetNameTag(node["nameTag"].asString());
			m_ptlights.push_back(plight);

		} else if (type == "dlight") { //DirLights type
			Json::Value dir = node["dir"];
			vec3f mdir(dir[0].asFloat(), dir[1].asFloat(), dir[2].asFloat());

			Json::Value color = node["color"];
			vec3f mcolor(color[0].asFloat(), color[1].asFloat(), color[2].asFloat());

			EXP_DirLight* dlight = new EXP_DirLight(m_game, mdir, mcolor, node.get("brightness", "1.0").asFloat());
			dlight->SetNameTag(node["nameTag"].asString());
			m_dlights.push_back(dlight);
		}
	}

	m_map.close();
	return true;
}

void EXP_MapLoader::UnloadMap() {

}