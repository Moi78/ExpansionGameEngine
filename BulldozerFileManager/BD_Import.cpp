#include "pch.h"
#include "BD_Import.h"

BD_Import::BD_Import() {

}

BD_Import::~BD_Import() {
	
}

void BD_Import::ClearAll() {
	m_filename = "";

	mVerticesInMem.clear();
	mIndicesInMem.clear();
	mNormalsInMem.clear();
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

vec3d BD_Import::GetNormalInMemByIndex(int index) {
	if (index > mVerticesInMem.size()) {
		dispErrorMessageBox(TEXT("Given index is out of range. See console for details."));
		std::cerr << "Error on getting normal in file " << m_filename << ". Give index is out of range." << std::endl;
		return vec3d(0.0, 0.0, 0.0);
	}

	return mNormalsInMem[index];
}

int BD_Import::GetVertexInMemCount() {
	return mVerticesInMem.size();
}

int BD_Import::GetIndicesInMemCount() {
	return mIndicesInMem.size();
}

int BD_Import::GetNormalInMemCount() {
	return mNormalsInMem.size();
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
	int nbrNormal = mNormalsInMem.size();

	bFile.write(reinterpret_cast<const char*>(&nbrVertices), sizeof(int));
	
	for (int i = 0; i < nbrVertices; i++) {
		bFile.write(reinterpret_cast<const char*>(&mVerticesInMem[i]), sizeof(vec3d));
	}

	bFile.write(reinterpret_cast<const char*>(&nbrIndice), sizeof(int));

	for (int i = 0; i < nbrIndice; i++) {
		bFile.write(reinterpret_cast<const char*>(&mIndicesInMem[i]), sizeof(int));
	}

	bFile.write(reinterpret_cast<const char*>(&nbrNormal), sizeof(int));

	for (int i = 0; i < nbrNormal; i++) {
		bFile.write(reinterpret_cast<const char*>(&mNormalsInMem[i]), sizeof(vec3d));
	}

	bFile.close();
}

void BD_Import::AppendIndice(int indice) {
	mIndicesInMem.push_back(indice);
}

void BD_Import::AppendVertex(vec3d vertex) {
	mVerticesInMem.push_back(vertex);
}

void BD_Import::AppendNormal(vec3d normal) {
	mNormalsInMem.push_back(normal);
}

void BD_Import::ImportFBX(std::string filepath, BD_FBX_ImportOption impOPT) {

}