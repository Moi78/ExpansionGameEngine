#include "pch.h"
#include "BD_Reader.h"

BD_Reader::BD_Reader() {
	icount = 0;
	vcount = 0;
	ncount = 0;
}

BD_Reader::~BD_Reader() {
	ClearAll();
}

void BD_Reader::ClearAll() {
	mIndices.clear();
	mVertices.clear();
	mNormal.clear();
	mUVcoord.clear();
}

void BD_Reader::ReadMSHFile(std::string file) {
	if (!std::filesystem::exists(file)) {
		const wchar_t* f = std::wstring(file.begin(), file.end()).c_str();
		dispErrorMessageBox(f);
		return;
	}

	//Opening bin file
	std::ifstream bFile;
	bFile.open(file.c_str(), std::ios::binary);

	//Error check
	if (!bFile) {
		dispErrorMessageBox(TEXT("Error while creating stream to a file. See console for details."));
		std::cout << "Error while create filestream to the file \"" << file << "\"." << std::endl;
	}

	int vSize = 0;
	int iSize = 0;
	int nSize = 0;
	int uvSize = 0;

	//Reading vertices
	bFile.read(reinterpret_cast<char*>(&vSize), sizeof(int)); //Reading Size

	vec3f temp(0.0, 0.0, 0.0); //Temp variable
	vec2f tempUV(0.0, 0.0); //Temp variable for UV coords

	for (int i = 0; i < vSize; i++) {								  //Reading
		bFile.read(reinterpret_cast<char*>(&temp), sizeof(vec3f));	  //Values
		mVertices.push_back(temp);									  //From file
	}

	//Reading indices
	bFile.read(reinterpret_cast<char*>(&iSize), sizeof(int)); //Same pattern

	int itemp = 0;

	for (int i = 0; i < iSize; i++) {
		bFile.read(reinterpret_cast<char*>(&itemp), sizeof(int));
		mIndices.push_back(itemp);
	}

	//Reading normals
	bFile.read(reinterpret_cast<char*>(&nSize), sizeof(int)); //Same pattern

	temp = vec3f(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < nSize; i++) {
		bFile.read(reinterpret_cast<char*>(&temp), sizeof(vec3f));
		mNormal.push_back(temp);
	}

	//Reading UV
	bFile.read(reinterpret_cast<char*>(&uvSize), sizeof(int)); //Same pattern

	for (int i = 0; i < uvSize; i++) {
		bFile.read(reinterpret_cast<char*>(&tempUV), sizeof(vec2f));
		mUVcoord.push_back(tempUV);
	}

	icount = iSize;
	vcount = vSize;
	ncount = nSize;
	uvcount = uvSize;

	bFile.close();
}

int BD_Reader::GetIndiceByIndex(int index) {
	//Error check
	if (index > mIndices.size()) {
		dispErrorMessageBox(TEXT("Index out of range. See console for details."));
		std::cerr << "(INDICES) Index is out of range." << std::endl;
	}

	return mIndices[index];
}

vec3f BD_Reader::GetVertexByIndex(int index) {
	//Error check
	if (index > mVertices.size()) {
		dispErrorMessageBox(TEXT("Index out of range. See console for details."));
		std::cerr << "(VERTICIES) Index is out of range." << std::endl;
	}

	return mVertices[index];
}

vec3f BD_Reader::GetNormalByIndex(int index) {
	//Error check
	if (index > mNormal.size()) {
		dispErrorMessageBox(TEXT("Index out of range. See console for details."));
		std::cerr << "(NORMAL) Index is out of range." << std::endl;
	}

	return mNormal[index];
}

vec2f BD_Reader::GetUVcoordByIndex(int index) {
	//Error check
	if (index > mUVcoord.size()) {
		dispErrorMessageBox(TEXT("Index out of range. See console for details."));
		std::cerr << "(UVs) Index is out of range." << std::endl;
	}

	return mUVcoord[index];
}

int BD_Reader::GetIndicesCount() {
	return icount;
}

int BD_Reader::GetVerticesCount() {
	return vcount;
}

int BD_Reader::GetNormalCount() {
	return ncount;
}

int BD_Reader::GetUVcoordCount() {
	return uvcount;
}
