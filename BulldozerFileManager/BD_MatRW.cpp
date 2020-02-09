#include "pch.h"
#include "BD_MatRW.h"

//Write

BD_MatWrite::BD_MatWrite() {

}

BD_MatWrite::~BD_MatWrite() {

}

bool BD_MatWrite::WriteMaterialToFile(BD_MatDef material, std::string filepath) {
	std::ofstream file;
	file.open(filepath, std::ios::binary);

	if (!file) {
		std::cerr << "Cannot create the file " << filepath << std::endl;
		dispErrorMessageBox(TEXT("Cannot create a file. See console for details..."));
		return false;
	}

	file.write(reinterpret_cast<const char*>(&material), sizeof(BD_MatDef));

	return true;
}

//Read

BD_MatRead::BD_MatRead() {

}

BD_MatRead::~BD_MatRead() {

}

BD_MatDef BD_MatRead::ReadMaterialFromFile(std::string filepath) {
	std::ifstream file;
	file.open(filepath, std::ios::binary);

	if (!file) {
		std::cerr << "Cannot open the file " << filepath << std::endl;
		dispErrorMessageBox(TEXT("Cannot open a file, see console for details... Returning void MatDef."));
		return {};
	}

	BD_MatDef buffer = {};
	file.read(reinterpret_cast<char*>(&buffer), sizeof(BD_MatDef));

	return buffer;
}