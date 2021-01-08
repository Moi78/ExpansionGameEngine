#include "pch.h"
#include "BD_MatRW.h"

//Write

BD_MatWrite::BD_MatWrite() {

}

BD_MatWrite::~BD_MatWrite() {

}

bool BD_MatWrite::WriteMaterialToFile(BD_WriteMdef material, std::string filepath) {
	std::ofstream file;
	file.open(filepath, std::ios::binary);

	if (!file) {
		std::cerr << "Cannot create the file " << filepath << std::endl;
		dispErrorMessageBox(TEXT("Cannot create a file. See console for details..."));
		return false;
	}

	
	Json::Value root;

	if (material.BaseColorRef != "") {
		root["BaseColor"] = material.BaseColorRef.c_str();
	}
	else {
		Json::Value ColorVec;
		ColorVec.append(material.BaseColor.getX());
		ColorVec.append(material.BaseColor.getY());
		ColorVec.append(material.BaseColor.getZ());
		root["BaseColor"] = ColorVec;
	}

	if (material.SpecularColorRef != "") {
		root["Specular"] = material.SpecularColorRef.c_str();
	}
	else {
		Json::Value ColorVec;
		ColorVec.append(material.SpecularColor.getX());
		ColorVec.append(material.SpecularColor.getY());
		ColorVec.append(material.SpecularColor.getZ());
		root["Specular"] = ColorVec;
	}

	root["NormalMap"] = material.NormalMap;
	root["Shininess"] = material.Shininess;

	file << root;

	file.close();
	return true;
}

//Read

BD_MatRead::BD_MatRead() {

}

BD_MatRead::~BD_MatRead() {

}

BD_WriteMdef BD_MatRead::ReadMaterialFromFile(std::string filepath) {
	std::ifstream file;
	file.open(filepath, std::ios::binary);

	if (!file) {
		std::cerr << "Cannot open the file " << filepath << std::endl;
		dispErrorMessageBox(TEXT("Cannot open a file, see console for details... Returning void MatDef."));
		return {};
	}

	Json::Value root;
	JSONCPP_STRING errs;

	Json::CharReaderBuilder crb;
	crb["collectComment"] = false;
	if (!Json::parseFromStream(crb, file, &root, &errs)) {
		std::cerr << "Cannot read material file \"" << filepath << "\". " << errs << std::endl;
		return {};
	}

	BD_WriteMdef buffer = {};
	
	if ((int)root["BaseColor"].type() == 6) {
		vec3f Color(root["BaseColor"][0].asFloat(), root["BaseColor"][1].asFloat(), root["BaseColor"][2].asFloat());
		buffer.BaseColor = Color;
	}
	else {
		buffer.BaseColorRef = root["BaseColor"].asString();
	}

	float shininess = root["Shininess"].asFloat();

	if ((int)root["Specular"].type() == 6) {
		vec3f Color(root["Specular"][0].asFloat(), root["Specular"][1].asFloat(), root["Specular"][2].asFloat());
		buffer.SpecularColor = Color;
	}
	else {
		buffer.SpecularColorRef = root["Specular"].asString();
	}

	buffer.NormalMap = root.get("NormalMap", "").asString();
	buffer.Shininess = shininess;

	return buffer;
}

//Custom Shader Read

BD_MatCustomShaderRead::BD_MatCustomShaderRead(std::string MatFile) {
	m_file = std::ifstream(MatFile);
	if (!m_file) {
		std::cerr << "Cannot open the file " << MatFile << std::endl;
		dispErrorMessageBox(TEXT("Cannot open a file, see console for details... Returning void MatDef."));
	}

	JSONCPP_STRING errs;

	Json::CharReaderBuilder crb;
	crb["collectComment"] = false;
	if (!Json::parseFromStream(crb, m_file, &m_root, &errs)) {
		std::cerr << "Cannot read material file. " << errs << std::endl;
	}
}

BD_MatCustomShaderRead::~BD_MatCustomShaderRead() {};

void BD_MatCustomShaderRead::CloseFile() {
	if (m_file)
		m_file.close();
}

std::string BD_MatCustomShaderRead::GetShaderCode() {
	return m_root["glsl"].asString();
}

int BD_MatCustomShaderRead::GetTextureCount() {
	return m_root["textures"].size();
}

std::string BD_MatCustomShaderRead::GetTextureParamName(int index) {
	return m_root["textures"][index]["param"].asString();
}

std::string BD_MatCustomShaderRead::GetTexturePath(int index) {
	return m_root["textures"][index]["path"].asString();
}

// BD_MatCustomShaderWrite

BD_MatCustomShaderWrite::BD_MatCustomShaderWrite() {

}

BD_MatCustomShaderWrite::~BD_MatCustomShaderWrite() {

}

void BD_MatCustomShaderWrite::AddTextureRef(std::string ref, std::string param) {
	m_texs.push_back(std::pair<std::string, std::string>(ref, param));
}

void BD_MatCustomShaderWrite::SetShaderCode(std::string shadercode) {
	m_shader_code = shadercode;
}

bool BD_MatCustomShaderWrite::WriteMaterialFile(std::string path) {
	std::ofstream file;
	file.open(path, std::ios::binary);
	if (!file) {
		std::cerr << "Cannot create the file " << path << std::endl;
		dispErrorMessageBox(TEXT("Cannot open a file, see console for details..."));
		return false;
	}

	Json::Value root;

	int index = 0;
	for (auto tex : m_texs) {
		root["textures"][index]["path"] = tex.first;
		root["textures"][index]["param"] = tex.second;
		index++;
	}

	root["glsl"] = m_shader_code;

	file << root;

	return true;
}