#pragma once

#ifndef _BD_IMPORT_H__
#define _BD_IMPORT_H__

#ifdef BULLDOZERFILEMANAGER_EXPORTS
#define BULLDOZERFILEMANAGER_API __declspec(dllexport)
#else
#define BULLDOZERFILEMANAGER_API __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "BulldozerFileManager.h"
#include "BD_StructMan.h"

#include "vec3.h"

#include <fbxsdk.h>
#include <fbxsdk/fileio/fbxiosettings.h>
#include <fbxsdk/utils/fbxgeometryconverter.h>

class BULLDOZERFILEMANAGER_API BD_Import
{
public:
	BD_Import();
	~BD_Import();

	void ImportFBX(std::string, BD_FBX_ImportOption);
	void ImportImageFile(std::string filepath);

	int GetIndiceInMemByIndex(int index);
	vec3d GetVertexInMemByIndex(int index);

	int GetVertexInMemCount();
	int GetIndicesInMemCount();

	void ToBinary(std::string, std::string);

private:
	void ClearAll();

	void InitFBX_SDK();

	void ExtractMeshFromScene(FbxScene* scene);
	void GetMeshInfo(int index);
	void ClearNodeArray();

	int GetNodeCount();

	std::string m_filename;

	FbxManager* lSdkManager;
	FbxIOSettings* ios;
	FbxImporter* lImporter;

	std::vector<FbxNode*> pNodes;

	std::vector<vec3d> mVerticesInMem;
	std::vector<int> mIndicesInMem;
};
#endif