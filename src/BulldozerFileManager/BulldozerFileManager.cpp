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
	std::string data = "";

	std::ifstream file;
	file.open(filePath);

	if (!file) {
		dispErrorMessageBox(TEXT("Cannot open file, see console."));
		std::cerr << "File : " << filePath << " cannot be read, returning \"\"." << std::endl;
		return "";
	}

	std::string tmp;

	while (file.eof()) {
		std::getline(file, tmp);
		data.append(tmp);
	}

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
