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