#ifndef _BD_READER_H__
#define _BD_READER_H__

#include <vec.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

class BD_Reader
{
public:
	BD_Reader();
	~BD_Reader();

	void ReadMSHFile(std::string);

	int GetIndiceByIndex(int);
	vec3 GetVertexByIndex(int);
	vec3 GetNormalByIndex(int);
	vec2 GetUVcoordByIndex(int);
	vec4 GetVertexWeightByIndex(int);
    vec4 GetBoneIDByIndex(int);
    vec3 GetTanByIndex(int);
    vec3 GetBitanByIndex(int);

	int GetIndicesCount();
	int GetVerticesCount();
	int GetNormalCount();
	int GetUVcoordCount();
	int GetVertexWeightCount();
    int GetBoneIDCount();
    int GetTanBitanCount();

	void ClearAll();

private:
	std::vector<int> mIndices;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormal;
	std::vector<vec2> mUVcoord;
	std::vector<vec4> mVertWeight;
    std::vector<vec4> mBoneId;
    std::vector<vec3> mTan;
    std::vector<vec3> mBitan;

	int icount;
	int vcount;
	int ncount;
	int uvcount;
	int vwcount;
    int bidcount;
    int tan_bitan_count;
};

#endif // _BD_READER_H__