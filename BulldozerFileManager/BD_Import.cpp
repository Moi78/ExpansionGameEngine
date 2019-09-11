#include "pch.h"
#include "BD_Import.h"

BD_Import::BD_Import() {
	m_filename = "";

	InitFBX_SDK();
}

BD_Import::~BD_Import() {
	lSdkManager->Destroy();
}

void BD_Import::ClearAll() {
	m_filename = "";

	mVerticesInMem.clear();
	mIndicesInMem.clear();
}

void BD_Import::ImportFBX(std::string filepath, BD_FBX_ImportOption impOpt) {
	FbxScene* scene = FbxScene::Create(lSdkManager, "scene");
	m_filename = filepath;

	lImporter = FbxImporter::Create(lSdkManager, "");

	lSdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, impOpt.ImportMaterials);
	lSdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, impOpt.ImportTextures);
	lSdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, impOpt.ImportAnimations);
	lSdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, impOpt.ImportGlobalSettings);

	const char* c_filepath = filepath.c_str();

	bool importStatus = lImporter->Initialize(c_filepath, -1, lSdkManager->GetIOSettings());

	if (!importStatus) {
		dispErrorMessageBox(TEXT("Cannot initialize FBX import. See console for details."));
		std::cerr << "FBX Import fail, while initializing importer, cannot import " << filepath << std::endl;

		return;
	}

	lImporter->Import(scene);

	int versionM, versionm, revision;

	lImporter->GetFileVersion(versionM, versionm, revision);

	std::cout << "Successfully imported file \"" << filepath << "\"." << " File version : " << versionM << " File revision : " << revision << std::endl;

	FbxGeometryConverter* converter = new FbxGeometryConverter(lSdkManager);
	converter->Triangulate(scene, true);

	ExtractMeshFromScene(scene);
	GetMeshInfo(0);

	lImporter->Destroy();

	return;
}

void BD_Import::InitFBX_SDK() {
	lSdkManager = FbxManager::Create();
	ios = FbxIOSettings::Create(lSdkManager, IOSROOT);

	lSdkManager->SetIOSettings(ios);
}

void BD_Import::ExtractMeshFromScene(FbxScene* scene) {
	FbxNode* RootNode = scene->GetRootNode();

	if (!RootNode) {
		dispErrorMessageBox(TEXT("Cannot get root node. See console for details"));
		std::cerr << "FBX Error : Cannot extract root node from file " << m_filename << "." << std::endl;
		
		return;
	}

	std::cout << RootNode->GetChildCount() << " Childs found." << std::endl;

	for (int i = 0; i < RootNode->GetChildCount(); i++) {
		pNodes.push_back(RootNode->GetChild(i));
	}

	ClearNodeArray();

	return;
}

void BD_Import::GetMeshInfo(int index) {
	if (index > pNodes.size()) {
		dispErrorMessageBox(TEXT("Index out of range. See console for details."));
		std::cerr << "Cannot extract mesh info because the given index is out of range. Source file : " << m_filename << std::endl;
	}

	FbxNode* toGet = pNodes[index];

	FbxMesh* mesh = toGet->GetMesh();

	int vertexCount = mesh->GetControlPointsCount();
	int PolyCount = mesh->GetPolygonCount();

	for (int i = 0; i < vertexCount; i++) {
		mVerticesInMem.push_back(vec3d(mesh->GetControlPointAt(i).mData[0], mesh->GetControlPointAt(i).mData[1], mesh->GetControlPointAt(i).mData[2]));
	}

	for (int i = 0; i < PolyCount; i++) {
		mIndicesInMem.push_back(mesh->GetPolygonVertex(i, 0));
		mIndicesInMem.push_back(mesh->GetPolygonVertex(i, 1));
		mIndicesInMem.push_back(mesh->GetPolygonVertex(i, 2));
	}

	toGet->Destroy();

	return;
}

void BD_Import::ClearNodeArray() {
	for (int i = 0; i < pNodes.size(); i++)  {
		if (pNodes[i]->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh) {
			pNodes.erase(pNodes.begin() + i);
		}
	}
}

int BD_Import::GetNodeCount() {
	return pNodes.size();
}

int BD_Import::GetIndiceInMemByIndex(int index) {
	if (index > mIndicesInMem.size()) {
		dispErrorMessageBox(TEXT("Given index is out of range. See console for details."));
		std::cerr << "Error on getting indices in file " << m_filename << ". Give index is out of range." << std::endl;
		return 0;
	}
	
	return mIndicesInMem[index];
}

vec3d BD_Import::GetVertexInMemByIndex(int index) {
	if (index > mVerticesInMem.size()) {
		dispErrorMessageBox(TEXT("Given index is out of range. See console for details."));
		std::cerr << "Error on getting vertex in file " << m_filename << ". Give index is out of range." << std::endl;
		return vec3d(0.0, 0.0, 0.0);
	}

	return mVerticesInMem[index];
}

int BD_Import::GetVertexInMemCount() {
	return mVerticesInMem.size();
}

int BD_Import::GetIndicesInMemCount() {
	return mIndicesInMem.size();
}

void BD_Import::ToBinary(std::string destDir, std::string filename) {

	std::string ddir = destDir + filename + ".msh";

	std::ofstream bFile;
	bFile.open(ddir.c_str(), std::ios::binary);

	if (!bFile) {
		dispErrorMessageBox(TEXT("Cannot create bin file. See console for details."));
		std::cerr << "Cannot create bin file : " << ddir;

		return;
	}

	int nbrVertices = GetVertexInMemCount();
	int nbrIndice = GetIndicesInMemCount();

	std::cout << nbrIndice << std::endl;

	bFile.write(reinterpret_cast<const char*>(&nbrVertices), sizeof(int));
	
	for (int i = 0; i < nbrVertices; i++) {
		bFile.write(reinterpret_cast<const char*>(&mVerticesInMem[i]), sizeof(vec3d));
	}

	bFile.write(reinterpret_cast<const char*>(&nbrIndice), sizeof(int));

	for (int i = 0; i < nbrIndice; i++) {
		bFile.write(reinterpret_cast<const char*>(&mIndicesInMem[i]), sizeof(int));
	}

	bFile.close();
}