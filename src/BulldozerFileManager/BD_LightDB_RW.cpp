#include "pch.h"
#include "BD_LightDB_RW.h"

//------------------- WRITE -------------------

BD_LightDB_Write::BD_LightDB_Write() {

}

BD_LightDB_Write::~BD_LightDB_Write() {
	for (auto c : m_components) {
		free(c.ShadowMap);
	}
}

void BD_LightDB_Write::AddComponent(BD_LDB_Comp comp) {
	m_components.push_back(comp);
}

bool BD_LightDB_Write::WriteAllComps(std::string filename) {
	std::string fullpath = filename + ".ldb";

	std::ofstream file;
	file.open(fullpath, std::ios::binary);
	if (!file) {
		std::cerr << "ERROR: Cannot open stream to file." << std::endl;
		return false;
	}

	//Number of component
	int nbrComp = m_components.size();
	file.write(reinterpret_cast<const char*>(&nbrComp), sizeof(int));

	for (auto c : m_components) {
		//Comp name
		int nameSize = c.CompName.size();
		file.write(reinterpret_cast<const char*>(&nameSize), sizeof(int));

		file.write(c.CompName.c_str(), nameSize);

		//Comp shadow map size
		int smSize = c.ShadowMapSize;
		file.write(reinterpret_cast<const char*>(&smSize), sizeof(int));

		//Comp shadow map data
		file.write(reinterpret_cast<const char*>(c.ShadowMap), 3 * smSize);
	}

	std::cout << "Wrote database to \"" << fullpath << "\"" << std::endl;
	return true;
}

//------------------- READ -------------------

BD_LightDB_Read::BD_LightDB_Read() {

}

BD_LightDB_Read::~BD_LightDB_Read() {

}

bool BD_LightDB_Read::ReadDB(std::string file) {
	m_components.clear();

	std::cout << "Opening database at \"" << file << "\"" << std::endl;
	
	if (!std::filesystem::exists(std::filesystem::path(file))) {
		std::cerr << "ERROR: File \"" << file << "\" does not exist." << std::endl;
		return false;
	}

	//Opening file
	std::ifstream bFile;
	bFile.open(file, std::ios::binary);
	if (!bFile) {
		std::cerr << "ERROR: Cannot open a stream to your file." << std::endl;
		return false;
	}

	//Getting comp size
	int cSize = 0;
	bFile.read(reinterpret_cast<char*>(&cSize), sizeof(int));

	for (int i = 0; i < cSize; i++) {
		BD_LDB_Comp c;

		//Getting component name
		int nameSize = 0;
		bFile.read(reinterpret_cast<char*>(&nameSize), sizeof(int));

		char* name = (char*)malloc(nameSize + 1);
		memset(name, 0, nameSize + 1);
		bFile.read(reinterpret_cast<char*>(name), nameSize);

		assert(name != NULL && "BAD READ");

		c.CompName = name;

		//Getting componenent shadow map size
		int smSize = 0;
		bFile.read(reinterpret_cast<char*>(&smSize), sizeof(int));

		c.ShadowMapSize = smSize;

		void* smData = malloc(3 * smSize);
		bFile.read(reinterpret_cast<char*>(smData), 3 * smSize);

		c.ShadowMap = smData;

		m_components.push_back(c);
	}

	return true;
}

int BD_LightDB_Read::GetCompCount() {
	return m_components.size();
}

BD_LDB_Comp BD_LightDB_Read::GetCompByIndex(int index) {
	assert(index < GetCompCount() && "BAD INDEX");

	return m_components[index];
}