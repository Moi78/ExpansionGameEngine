#pragma once

#ifndef _RD_MATERIAL_LIBRARY_H__
#define _RD_MATERIAL_LIBRARY_H__

#include <vector>
#include <string>

#include <BD_StructMan.h>

#include "RaindropRenderer.h"

class RAINDROPRENDERER_API RD_MaterialLibrary
{
public:
	RD_MaterialLibrary();
	~RD_MaterialLibrary();

	void AddMaterialToLib(RD_ShaderMaterial* mat, std::string name);
	void RemoveMaterialFromLib(std::string matName);
	void ClearLibrary();

	bool DoMaterialExists(std::string nameDenominator);
	RD_ShaderMaterial* GetMaterialByName(std::string nameDenominator);

private:
	std::vector<std::pair<std::string, RD_ShaderMaterial*>> m_materials;
};

#endif