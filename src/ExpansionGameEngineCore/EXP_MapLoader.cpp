#include "pch.h"
#include "EXP_MapLoader.h"

#include "EXP_Level.h"

EXP_MapLoader::EXP_MapLoader(EXP_Game* game) {
	m_game = game;
}

EXP_MapLoader::~EXP_MapLoader() {

}

bool EXP_MapLoader::LoadMap(std::string map) {
	if (!std::filesystem::exists(map)) {
		std::cerr << "Cannot load map file, file " << map << " does not exists" << std::endl;
		return false;
	}

	std::ifstream mapStream(map.c_str());

	Json::Value root;
	JSONCPP_STRING errs;

	std::cout << "Loading map" << std::endl;

	Json::CharReaderBuilder builder;
	builder["collectComment"] = false;

	if (!Json::parseFromStream(builder, mapStream, &root, &errs)) {
		std::cerr << "Cannot load map file " << map << ". " << errs << std::endl;
		return false;
	}

	//Getting Level code object from user's shared lib (Handler)
	std::string MapCodeObject = root["MapLevelCodeObjectName"].asString() + "Handler";
	LEVELCODEHANDLER lvlH = m_game->GetGameLib()->FetchLibHandler<LEVELCODEHANDLER>(MapCodeObject.c_str());
	if (lvlH == NULL) {
		std::cerr << "Cannot load code object " << root["MapLevelCodeObjectName"].asString() << std::endl;
		exit(-3);
	}
	else {
		m_rawLevelCode = lvlH(m_game, this);
		m_levelCode = (EXP_Level*)m_rawLevelCode;
        if(!m_levelCode) {
            std::cerr << "ERROR: Cannot create instance of level code object. " << MapCodeObject << std::endl;
            exit(-4);
        } else {
            std::cout << "Instanciated level code object. " << MapCodeObject << std::endl;
        }
	}

	//Getting Level code object from user's shared lib (Remover)
	std::string MapCodeObjRl = root["MapLevelCodeObjectName"].asString() + "Remover";
	m_rl = m_game->GetGameLib()->FetchLibHandler<LEVELCODERELEASER>(MapCodeObjRl.c_str());
	if (m_rl == NULL) {
		std::cerr << "Cannot load code object" << root["MapLevelCodeObjectName"] << std::endl;
		exit(-3);
	}

	int nodeCount = root["nodes"].size();
	for (int i = 0; i < nodeCount; i++) {

		Json::Value node = root["nodes"][i];
		std::string type = node.get("type", "mesh").asString();

		//Creating map by specified type
		//Actors must be spawned from a EXP_Level child class

		if (type == "mesh") { //Mesh type
			std::cout << "Registering mesh" << std::endl;

			std::string ref = node.get("ref", "/").asString();

			std::string mat = node.get("material", "").asString();
			RD_ShaderMaterial* shader = m_game->GetRenderer()->FetchShaderFromFile(m_game->GetFilePathByRef(mat) + ".exmtl");

			Json::Value pos = node["pos"];
			vec3f mpos(pos[0].asFloat(), pos[1].asFloat(), pos[2].asFloat());

			Json::Value rot = node["rot"];
			vec3f mrot(rot[0].asFloat(), rot[1].asFloat(), rot[2].asFloat());

			Json::Value scale = node["scale"];
			vec3f mscale(scale[0].asFloat(), scale[1].asFloat(), scale[2].asFloat());

			EXP_StaticMesh* mesh = new EXP_StaticMesh(m_game, shader, ref, mpos, mrot, mscale);
			mesh->SetNameTag(node["nameTag"].asString());
			m_meshes.push_back(mesh);

		} else if (type == "plight") { //PointLight type
			std::cout << "Registering point light." << std::endl;

			Json::Value pos = node["pos"];
			vec3f mpos(pos[0].asFloat(), pos[1].asFloat(), pos[2].asFloat());

			Json::Value color = node["color"];
			vec3f mcolor(color[0].asFloat(), color[1].asFloat(), color[2].asFloat());

			Json::Value brightness = node.get("brightness", "1.0");
			Json::Value radius = node.get("radius", "5.0");

			EXP_PointLight* plight = new EXP_PointLight(m_game, mpos, mcolor, brightness.asFloat(), radius.asFloat());
			plight->SetNameTag(node["nameTag"].asString());
			m_ptlights.push_back(plight);

		} else if (type == "dlight") { //DirLights type
			std::cout << "Registering directional light" << std::endl;

			Json::Value dir = node["dir"];
			vec3f mdir(dir[0].asFloat(), dir[1].asFloat(), dir[2].asFloat());

			Json::Value color = node["color"];
			vec3f mcolor(color[0].asFloat(), color[1].asFloat(), color[2].asFloat());

			EXP_DirLight* dlight = new EXP_DirLight(m_game, mdir, mcolor, node.get("brightness", "1.0").asFloat());
			dlight->SetNameTag(node["nameTag"].asString());
			m_dlights.push_back(dlight);
		}
	}

	mapStream.close();
	return true;
}

void EXP_MapLoader::UnloadMap() {
	std::cout << "Unloading map." << std::endl;

	for (auto mesh : m_meshes) {
		std::cout << "Unloading mesh" << std::endl;

		m_game->UnregisterMesh(mesh);
		delete mesh;
	}
	m_meshes.clear();

	for (auto dlight : m_dlights) {
		std::cout << "Unloading dir light" << std::endl;

		m_game->UnregisterDirLight(dlight);
	}
	m_dlights.clear();

	for (auto plight : m_ptlights) {
		std::cout << "Unloading point light" << std::endl;

		m_game->UnregisterPointLight(plight);
	}
	m_ptlights.clear();

	if (m_levelCode) { //If there is a level code object, call it destructor and set it to nullptr
		std::cout << "Unloading level code object" << std::endl;

		m_rl(m_rawLevelCode);
		m_levelCode = nullptr;
	}
}

EXP_Level* EXP_MapLoader::GetLevelCode() {
	if (m_levelCode) {
		return m_levelCode;
	}
	else {
		return nullptr;
	}
}

EXP_StaticMesh* EXP_MapLoader::GetStaticMeshByName(std::string name) {
	for (auto mesh : m_meshes) {
		if (mesh->GetNameTag() == name) {
			return mesh;
		}
	}

	std::cerr << "ERROR: StaticMesh " << name << " was not found. Returning nullptr." << std::endl;
	return nullptr;
}

EXP_PointLight* EXP_MapLoader::GetPointLightByName(std::string name) {
	for (auto plight : m_ptlights) {
		if (plight->GetNameTag() == name) {
			return plight;
		}
	}

	std::cerr << "ERROR: PointLight " << name << " was not found. Returning nullptr." << std::endl;
	return nullptr;
}

EXP_DirLight* EXP_MapLoader::GetDirLightByName(std::string name) {
	for (auto dlight : m_dlights) {
		if (dlight->GetNameTag() == name) {
			return dlight;
		}
	}

	std::cerr << "ERROR: DirLight " << name << " was not found. Returning nullptr." << std::endl;
	return nullptr;
}
