#include "pch.h"
#include "BD_Reader.h"

BD_Reader::BD_Reader() {

}

BD_Reader::~BD_Reader() {

}

void BD_Reader::ClearAll() {
	mIndices.clear();
	mVertices.clear();
}

void BD_Reader::ReadMSHFile(std::string file) {
	std::ifstream bFile;
	bFile.open(file.c_str(), std::ios::binary);

	if (!bFile) {
		dispErrorMessageBox(TEXT("Error while creating stream to a file. See console for details."));
		std::cout << "Error while create filestream to the file \"" << file << "\"." << std::endl;
	}

	int vSize;
	int iSize;

	std::vector<vec3d> vertices;
	std::vector<int> indices;

	bFile.read(reinterpret_cast<char*>(&vSize), sizeof(int));

	vec3d temp(0.0, 0.0, 0.0);

	for (int i = 0; i < vSize; i++) {
		bFile.read(reinterpret_cast<char*>(&temp), sizeof(vec3d));
		mVertices.push_back(temp);
	}

	bFile.read(reinterpret_cast<char*>(&iSize), sizeof(int));

	int itemp = 0;

	for (int i = 0; i < iSize; i++) {
		bFile.read(reinterpret_cast<char*>(&itemp), sizeof(int));
		mIndices.push_back(itemp);
	}

	icount = iSize;
	vcount = vSize;

	bFile.close();
}

int BD_Reader::GetIndiceByIndex(int index) {
	if (index > mIndices.size()) {
		dispErrorMessageBox(L"Index out of range. See console for details.");
		std::cerr << "Index is out of range." << std::endl;
	}

	return mIndices[index];
}

vec3d BD_Reader::GetVertexByIndex(int index) {
	if (index > mVertices.size()) {
		dispErrorMessageBox(L"Index out of range. See console for details.");
		std::cerr << "Index is out of range." << std::endl;
	}

	return mVertices[index];
}

int BD_Reader::GetIndicesCount() {
	return icount;
}

int BD_Reader::GetVerticesCount() {
	return vcount;
}