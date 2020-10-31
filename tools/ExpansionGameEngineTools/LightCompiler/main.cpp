#include <iostream>
#include <string>
#include <filesystem>

#include <BD_LightDB_RW.h>

#include "LevelAnalyser.h"

#define VERSION_STR "0.1"

int main(int argc, char* argv[]) {
	std::cout << "EXPANSION GAME ENGINE STATIC LIGHT COMPILER v" << VERSION_STR << std::endl;

	std::string levelFilePath;
	std::string workingDir;
	std::string outPath;

	if (argv[1] == NULL) {
		std::cerr << "No level file specified" << std::endl;
		exit(-1);
	}
	else {
		levelFilePath = argv[1];
		if (!std::filesystem::exists(std::filesystem::path(levelFilePath))) {
			std::cerr << "ERROR: \"" << levelFilePath << "\", no such file or directory" << std::endl;
			exit(-1);
		}

		std::cout << "Compiling light for " << levelFilePath << " level." << std::endl;
	}

	if (argv[2] == NULL) {
		std::cout << "WARNING: No outpath specified, outputting as \"out.ldb\"" << std::endl;
		outPath = "out";
	}
	else {
		std::cout << "Out file : \"" << argv[2] << ".ldb" << "\"" << std::endl;
		outPath = argv[2];
	}

	if (argv[3] == NULL) {
		std::cout << "WARNING: No working directory specified, using default working directory." << std::endl;
		workingDir = "./";
	}
	else {
		if (!std::filesystem::exists(std::filesystem::path(argv[3]))) {
			std::cerr << "ERROR: Working directory does not exists, using default working directory." << std::endl;
			workingDir = "./";
		}
		else {
			std::cout << "Working directory : \"" << argv[3] << "\"" << std::endl;
			workingDir = argv[3];
		}
	}

	LevelAnalyser* analyser = new LevelAnalyser();
	analyser->AnalyseLevel(levelFilePath);

	//Shadow calculations . . .

	analyser->WriteComponents(outPath);

	return 0;
}