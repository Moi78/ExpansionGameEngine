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

	int GetIndicesCount();
	int GetVerticesCount();
	int GetNormalCount();
	int GetUVcoordCount();
	int GetVertexWeightCount();

	void ClearAll();

private:
	std::vector<int> mIndices;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormal;
	std::vector<vec2> mUVcoord;
	std::vector<vec4> mVertWeight;

	int icount;
	int vcount;
	int ncount;
	int uvcount;
	int vwcount;
};

#endif // _BD_READER_H__