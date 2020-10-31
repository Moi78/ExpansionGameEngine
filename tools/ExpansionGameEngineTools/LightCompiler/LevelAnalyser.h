#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <BD_LightDB_RW.h>

#include <vec3.h>

#include <json/json.h>

struct BasicMesh {
	std::string nameTag;

	std::string meshRef;
	vec3f pos;
	vec3f rot;
	vec3f scale;
};

class LevelAnalyser {
public:
	LevelAnalyser();
	~LevelAnalyser();

	void AnalyseLevel(std::string levelPath);
	//void MakeShadowMaps();

	void WriteComponents(std::string out);

private:
	std::vector<BasicMesh> m_comps;
	std::vector<vec3f> m_dlights;

	std::vector<BD_LDB_Comp> m_calculated_comps;
};