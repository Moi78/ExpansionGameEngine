#include "pch.h"
#include "framework.h"
#include "BulldozerFileManager.h"

#ifdef _WIN32

void dispErrorMessageBox(LPCWSTR Message) {
	MessageBox(nullptr, Message, TEXT("ERROR !!!"), MB_ICONERROR | MB_OK);
}

#else

void dispErrorMessageBox(std::wstring Message) {
	std::wcerr << Message << std::endl;
}

#endif //_WIN32

std::string getFileData(std::string filePath) {
	std::ifstream file;
	file.open(filePath);

	if (!file) {
		std::wstring f(filePath.begin(), filePath.end());
		dispErrorMessageBox(std::wstring(f + L" cannot be opened. \"\" will be returned.").c_str());
		return "";
	}

	std::string temp;

	std::string fileData = "";

	while (!file.eof()) {
		std::getline(file, temp);
		fileData.append(temp + "\n");
	}

	file.close();
	return fileData;
}

std::vector<uint8_t> getFileDataBin(std::string fileName) {
	std::ifstream file;
	file.open(fileName, std::ios::binary | std::ios::ate);
	if (!file) {
		std::wstring f(fileName.begin(), fileName.end());
		dispErrorMessageBox(std::wstring(f + L" cannot be opened. Empty vector will be returned.").c_str());
		return {};
	}

	size_t fileSize = file.tellg();
	std::vector<uint8_t> data(fileSize);

	file.seekg(0);
	file.read(reinterpret_cast<char*>(data.data()), fileSize);

	file.close();
	return data;
}

std::string getFileExtension(std::string fileName) {
	std::string extension;

	size_t nSize = fileName.size() - 1;

	for (int i = nSize; i > 0; i--) {
		if (fileName[i] != '.') {
			extension.push_back(fileName[i]);
		}
		else {
			break;
		}
	}

	std::cout << extension << std::endl;

	return extension;
}
