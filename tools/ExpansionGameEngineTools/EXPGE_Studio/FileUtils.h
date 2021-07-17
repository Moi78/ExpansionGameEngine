#pragma once
#include <vector>
#include <filesystem>
#include <string>

enum class ElemType {
	FILE = 0,
	FOLDER = 1
};

std::string GetExtension(std::string name);
std::vector<std::pair<std::string, ElemType>> ListFilesAndFolders(std::string path, std::vector<std::string> filters);