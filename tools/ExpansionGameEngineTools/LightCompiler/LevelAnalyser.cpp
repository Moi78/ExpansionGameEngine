#include "LevelAnalyser.h"

LevelAnalyser::LevelAnalyser() {

}

LevelAnalyser::~LevelAnalyser() {

}

void LevelAnalyser::AnalyseLevel(std::string lvlPath) {
	std::cout << "Analysing level . . ." << std::endl;

	std::ifstream lvl;
	lvl.open(lvlPath);
	if (!lvl) {
		std::cerr << "ERROR: Cannot open stream to file \"" << lvlPath << "\"" << std::endl;
		exit(-1);
	}

	Json::Value root;
	JSONCPP_STRING errs;

	Json::CharReaderBuilder builder;
	builder["collectComment"] = false;

	if (!Json::parseFromStream(builder, lvl, &root, &errs)) {
		std::cerr << "ERROR: Cannot parse level file \"" << lvlPath << "\". " << errs << std::endl;
		exit(-1);
	}

	int nodeCount = root["nodes"].size();
	for (int i = 0; i < nodeCount; i++) {
		Json::Value node = root["nodes"][i];
		std::string type = node["type"].asString();

		if (type == "mesh") {
			BasicMesh m;

			m.nameTag = node["nameTag"].asString();
			m.meshRef = node["meshRef"].asString();

			Json::Value pos = node["pos"];
			m.pos = vec3f(pos[0].asFloat(), pos[1].asFloat(), pos[2].asFloat());

			Json::Value rot = node["rot"];
			m.rot = vec3f(rot[0].asFloat(), rot[1].asFloat(), rot[2].asFloat());

			Json::Value scale = node["scale"];
			m.scale = vec3f(scale[0].asFloat(), scale[1].asFloat(), scale[2].asFloat());

			m_comps.push_back(m);
		}

		if (type == "dlight") {
			m_dlights.push_back(vec3f(node["dir"][0].asFloat(), node["dir"][1].asFloat(), node["dir"][2].asFloat()));
		}
	}

	std::cout << "Found " << m_dlights.size() << " dir lights and " << m_comps.size() << " meshes." << std::endl;
}

void LevelAnalyser::WriteComponents(std::string out) {
	BD_LightDB_Write* ldb_w = new BD_LightDB_Write();

	for (auto c : m_comps) {
		BD_LDB_Comp nc;

		nc.CompName = c.nameTag;
		nc.ShadowMapSize = 16 * 16;

		nc.ShadowMap = malloc(16 * 16);

		ldb_w->AddComponent(nc);
	}

	ldb_w->WriteAllComps(out);

	delete ldb_w;
}