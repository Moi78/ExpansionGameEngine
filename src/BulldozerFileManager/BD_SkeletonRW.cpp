#include "pch.h"
#include "BD_SkeletonRW.h"

BD_SkeletonWrite::BD_SkeletonWrite() {

}

BD_SkeletonWrite::~BD_SkeletonWrite() {

}

void BD_SkeletonWrite::AddBone(const mat4f bone) {
	m_bones.push_back(bone);
}

void BD_SkeletonWrite::WriteBones(const std::string& filename) {
	std::ofstream file;
	file.open(filename, std::ios::binary);

	if (!file.is_open()) {
		std::string str = "Cannot create the file " + filename;
		std::wstring err(str.begin(), str.end());
		dispErrorMessageBox(err.c_str());
	}

	int s = m_bones.size();
	file.write(reinterpret_cast<char*>(&s), sizeof(int));
	for (auto b : m_bones) {
		float* f = b.GetPTR();
		file.write(reinterpret_cast<char*>(f), 16 * sizeof(float));
	}

	file.close();
}

//---------------------------------------------------------------------------------------

BD_SkeletonRead::BD_SkeletonRead() {

}

BD_SkeletonRead::~BD_SkeletonRead() {

}

void BD_SkeletonRead::ReadFile(const std::string& filename) {
	if (!std::filesystem::exists(filename)) {
		std::string str = "File " + filename + " does not exists.";
		std::wstring err(str.begin(), str.end());
		dispErrorMessageBox(err.c_str());
	}

	std::ifstream file;
	file.open(filename, std::ios::binary | std::ios::beg);

	if (!file.is_open()) {
		std::string str = "Cannot open " + filename;
		std::wstring err(str.begin(), str.end());
		dispErrorMessageBox(err.c_str());
	}

	int bone_count = 0;
	file.read(reinterpret_cast<char*>(&bone_count), sizeof(int));

	for (int i = 0; i < bone_count; i++) {
		mat4f b = mat4f();

		file.read(reinterpret_cast<char*>(b.GetPTR()), 16 * sizeof(float));
		m_bones.push_back(b);
	}

	file.close();
}

int BD_SkeletonRead::GetBoneCount() {
	return m_bones.size();
}

mat4f BD_SkeletonRead::GetBoneByIndex(int index) {
	assert(!(m_bones.size() < index));

	return m_bones[index];
}