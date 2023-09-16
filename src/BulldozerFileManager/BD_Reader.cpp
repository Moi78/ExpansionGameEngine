#include "BD_Reader.h"

BD_Reader::BD_Reader() {
	icount = 0;
	vcount = 0;
	ncount = 0;
    uvcount = 0;
    vwcount = 0;
    bidcount = 0;
    tan_bitan_count = 0;
}

BD_Reader::~BD_Reader() {
	ClearAll();
}

void BD_Reader::ClearAll() {
	mIndices.clear();
	mVertices.clear();
	mNormal.clear();
	mUVcoord.clear();
    mBoneId.clear();
    mVertWeight.clear();
    mBoneId.clear();
}

void BD_Reader::ReadMSHFile(std::string file) {
	if (!std::filesystem::exists(file)) {
		std::string message = file + " does not exists.";
        std::cerr << "ERROR: " << message << std::endl;
		return;
	}

	//Opening bin file
	std::ifstream bFile;
	bFile.open(file.c_str(), std::ios::binary);
    bFile.seekg(0);

	//Error check
	if (!bFile) {
		std::cout << "Error while create filestream to the file \"" << file << "\"." << std::endl;
	}

    uint32_t vSize = 0;
    uint32_t iSize = 0;
    uint32_t nSize = 0;
    uint32_t uvSize = 0;
    uint32_t vsSize = 0;
    uint32_t bidSize = 0;

	//Reading vertices
	bFile.read(reinterpret_cast<char*>(&vSize), sizeof(uint32_t)); //Reading Size

	vec3 temp(0.0, 0.0, 0.0); //Temp variable
	vec2 tempUV(0.0, 0.0); //Temp variable for UV coords

	for (int i = 0; i < vSize; i++) {								  //Reading
		bFile.read(reinterpret_cast<char*>(&temp), sizeof(vec3));	  //Values
		mVertices.push_back(temp);									  //From file
	}

	//Reading indices
	bFile.read(reinterpret_cast<char*>(&iSize), sizeof(uint32_t)); //Same pattern

	int itemp = 0;

	for (int i = 0; i < iSize; i++) {
		bFile.read(reinterpret_cast<char*>(&itemp), sizeof(uint32_t));
		mIndices.push_back(itemp);
	}

	//Reading normals
	bFile.read(reinterpret_cast<char*>(&nSize), sizeof(uint32_t)); //Same pattern

	temp = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < nSize; i++) {
		bFile.read(reinterpret_cast<char*>(&temp), sizeof(vec3));
		mNormal.push_back(temp);
	}

	//Reading UV
	bFile.read(reinterpret_cast<char*>(&uvSize), sizeof(uint32_t)); //Same pattern

	for (int i = 0; i < uvSize; i++) {
		bFile.read(reinterpret_cast<char*>(&tempUV), sizeof(vec2));
		mUVcoord.push_back(tempUV);
	}

	//Reading Vertex Strength
	bFile.read(reinterpret_cast<char*>(&vsSize), sizeof(uint32_t)); //Same Pattern

	vec4 value = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int i = 0; i < vsSize; i++) {
		bFile.read(reinterpret_cast<char*>(&value), sizeof(vec4));
		mVertWeight.push_back(value);
	}

    //Reading Bones ID for each verticies
    bFile.read(reinterpret_cast<char*>(&bidSize), sizeof(uint32_t));

    value = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for(int i = 0; i < bidSize; i++) {
        bFile.read(reinterpret_cast<char*>(&value), sizeof(vec4));
        mBoneId.push_back(value);
    }

    //Reading tan/bitan
    bFile.read((char*)&tan_bitan_count, sizeof(size_t));

    mTan.resize(tan_bitan_count);
    bFile.read((char*)mTan.data(), sizeof(vec3) * tan_bitan_count);

    mBitan.resize(tan_bitan_count);
    bFile.read((char*)mBitan.data(), sizeof(vec3) * tan_bitan_count);

	icount = iSize;
	vcount = vSize;
	ncount = nSize;
	uvcount = uvSize;
	vwcount = vsSize;
    bidcount = bidSize;

	bFile.close();
}

int BD_Reader::GetIndiceByIndex(int index) {
	//Error check
	if (index > mIndices.size()) {
		std::cerr << "(INDICES) Index is out of range." << std::endl;
        return 0;
	}

	return mIndices[index];
}

vec3 BD_Reader::GetVertexByIndex(int index) {
	//Error check
	if (index > mVertices.size()) {
		std::cerr << "(VERTICIES) Index is out of range." << std::endl;
        return vec3(0.0f, 0.0f, 0.0f);
	}

	return mVertices[index];
}

vec3 BD_Reader::GetNormalByIndex(int index) {
	//Error check
	if (index > mNormal.size()) {
		std::cerr << "(NORMAL) Index is out of range." << std::endl;
        return vec3(0.0f, 0.0f, 0.0f);
	}

	return mNormal[index];
}

vec2 BD_Reader::GetUVcoordByIndex(int index) {
	//Error check
	if (index > mUVcoord.size()) {
		std::cerr << "(UVs) Index is out of range." << std::endl;
        return vec2(0.0f, 0.0f);
	}

	return mUVcoord[index];
}

vec4 BD_Reader::GetVertexWeightByIndex(int index) {
	//Error check
	if (index > mVertWeight.size()) {
		std::cerr << "(VertexStrength) Index is out of range." << std::endl;
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return mVertWeight[index];
}

vec4 BD_Reader::GetBoneIDByIndex(int index) {
    //Error check
    if (index > mVertWeight.size()) {
        std::cerr << "(VertexStrength) Index is out of range." << std::endl;
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    return mBoneId[index];
}

vec3 BD_Reader::GetTanByIndex(int index) {
    //Error check
    if (index > mTan.size()) {
        std::cerr << "(Tan/Bitan) Index is out of range." << std::endl;
        return vec3(0.0f, 0.0f, 0.0f);
    }

    return mTan[index];
}

vec3 BD_Reader::GetBitanByIndex(int index) {
    //Error check
    if (index > mBitan.size()) {
        std::cerr << "(Tan/Bitan) Index is out of range." << std::endl;
        return vec3(0.0f, 0.0f, 0.0f);
    }

    return mBitan[index];
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

int BD_Reader::GetVertexWeightCount() {
	return vwcount;
}

int BD_Reader::GetBoneIDCount() {
    return bidcount;
}

int BD_Reader::GetTanBitanCount() {
    return tan_bitan_count;
}