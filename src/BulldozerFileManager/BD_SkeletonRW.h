#pragma once
#ifndef _BD_SKELETON_RW_H__
#define _BD_SKELETON_RW_H__

#ifdef _WIN32
	#ifdef BULLDOZERFILEMANAGER_EXPORTS
		#define BD_API __declspec(dllexport)
	#else
		#define BD_API __declspec(dllimport)
	#endif //BULLDOZERFILEMANAGER_EXPORTS
#else
	#define BD_API
#endif //_WIN32

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cassert>

#include <mat4.h>

#include "BulldozerFileManager.h"

/*
Skeleton file scheme :

INT bone_count

//Bones
Number of bones * (16*FLOAT bone_matrix)

*/

class BD_API BD_SkeletonWrite
{
public:
	BD_SkeletonWrite();
	~BD_SkeletonWrite();

	void AddBone(const mat4f bone);

	void WriteBones(const std::string& filename);

private:
	std::vector<mat4f> m_bones;
};

class BD_API BD_SkeletonRead {
public:
	BD_SkeletonRead();
	~BD_SkeletonRead();

	void ReadFile(const std::string& filename);

	int GetBoneCount();
	mat4f GetBoneByIndex(const int index);

private:
	std::vector<mat4f> m_bones;
};

#endif //_BD_SKELETON_RW_H__