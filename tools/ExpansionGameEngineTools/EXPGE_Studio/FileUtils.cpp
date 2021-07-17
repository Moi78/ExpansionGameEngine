#include "FileUtils.h"

std::string GetExtension(std::string name) {
	std::string buff;
	for (int i = name.size(); i >= 0; i--) {
		if (name[i] != '.') {
			buff = std::string(name.begin() + i, name.end());
		}
		else {
			break;
		}
	}

	return buff;
}

std::vector<std::pair<std::string, ElemType>> ListFilesAndFolders(std::string path, std::vector<std::string> filters) {
	try {
		if (std::filesystem::is_empty(path)) {
			return {};
		}
	}
	catch (std::exception& e) {
		return {};
	}

	//Iter over dir
	std::vector<std::pair<std::string, ElemType>> currentDirContent;

	for (auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory()) {
			currentDirContent.push_back(std::pair<std::string, ElemType>(entry.path().filename().string(), ElemType::FOLDER));
		}
		else {
			try {
				if (filters.size() > 0) {
					for (auto i : filters) {
						if (i == GetExtension(entry.path().filename().string()) && !entry.is_directory()) {
							currentDirContent.push_back(std::pair<std::string, ElemType>(entry.path().filename().string(), ElemType::FILE));
							break;
						}
					}
				}
				else {
					currentDirContent.push_back(std::pair<std::string, ElemType>(entry.path().filename().string(), ElemType::FILE));
				}
			}
			catch (std::exception& e) {
				/* Nothing to do as the error is probably something like "No UTF-8 Character" */
			}
		}
	}

	return currentDirContent;
}