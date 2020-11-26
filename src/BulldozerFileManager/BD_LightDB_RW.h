#pragma once

#ifdef _WIN32
	#ifdef BULLDOZERFILEMANAGER_EXPORTS
		#define BD_API __declspec(dllexport)
	#else //BULLDOZERFILEMANAGER_EXPORTS
		#define BD_API __declspec(dllimport)
	#endif //BULLDOZERFILEMANAGER_EXPORTS
#else //_WIN32
	#define BD_API
#endif //_WIN32

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <assert.h>
#include <cstring>

/*
* 
----------- LightDB (.ldb) file struct -----------

<Number of components>

<Component Name's Size> <Component Name>
<Component shadow map size (in pixel)>
<Component shadow map raw data>

//Same pattern
<. . .>

*/

struct BD_LDB_Comp {
	std::string CompName;

	int ShadowMapSize;
	void* ShadowMap;
};

class BD_API BD_LightDB_Write
{
public:
	BD_LightDB_Write();
	~BD_LightDB_Write();

	void AddComponent(BD_LDB_Comp nComp);

	bool WriteAllComps(std::string filename);

private:
	std::vector<BD_LDB_Comp> m_components;
};

class BD_API BD_LightDB_Read {
public:
	BD_LightDB_Read();
	~BD_LightDB_Read();

	bool ReadDB(std::string file);

	int GetCompCount();
	BD_LDB_Comp GetCompByIndex(int index);

private:
	std::vector<BD_LDB_Comp> m_components;
};