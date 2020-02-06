#include "pch.h"
#include "BD_ActorRW.h"

//Actor Write

BD_ActorWriter::BD_ActorWriter() {

}

BD_ActorWriter::~BD_ActorWriter() {

}

void BD_ActorWriter::SetRootComponent(std::string comp_label, BD_ComponentType type, std::string args) {
	BD_ComponentReference comp = {};
	comp.actor_component_index = 0;
	comp.type = type;
	comp.isRoot = true;
	comp.loc_pos = vec3f();
	comp.loc_rot = vec3f();
	comp.loc_scale = vec3f();
	comp.label = comp_label;
	comp.args = args;

	if (m_comps.size() != 0) {
		BD_ComponentReference buffer = m_comps[0];
		m_comps[0] = comp;
		m_comps.push_back(buffer);
	}
	else {
		m_comps.push_back(comp);
	}
}

void BD_ActorWriter::AddComponent(std::string comp_label, vec3f pos, vec3f rot, vec3f scale, BD_ComponentType type, std::string args) {
	BD_ComponentReference comp;
	comp.actor_component_index = m_comps.size();
	comp.type = type;
	comp.isRoot = false;
	comp.loc_pos = pos;
	comp.loc_rot = rot;
	comp.loc_scale = scale;
	comp.label = comp_label;
	comp.args = args;

	m_comps.push_back(comp);
}

void BD_ActorWriter::WriteActorFile(std::string actor_name) {
	std::string fullpath = actor_name + ".actor";

	std::ofstream file;
	file.open(fullpath.c_str(), std::ios::beg);

	if (!file) {
		std::cerr << "Cannot create actor file " << actor_name << std::endl;
		dispErrorMessageBox(TEXT("Cannot create actor file, see console for details."));
		return;
	}

	std::cout << "File successfully created." << std::endl;

	file << m_comps.size() << std::endl;

	for (auto comp : m_comps) {
		//Label Write
		file << "label:§" << comp.label << "§;";

		//LocPosScale
		file << comp.loc_pos.ConvertToParserFormat("pos") << comp.loc_rot.ConvertToParserFormat("rot") << comp.loc_scale.ConvertToParserFormat("scale");

		//isRoot
		file << "isRoot:" << comp.isRoot << ";";

		//Component Index
		file << "index:" << comp.actor_component_index << ";";

		//Component Type
		file << "type:" << static_cast<int>(comp.type) << ";";

		//Args
		file << "args:§" << comp.args << "§;" << std::endl;
	}

	file.close();
}

//Actor Read

BD_ActorReader::BD_ActorReader() {

}

BD_ActorReader::~BD_ActorReader() {

}

void BD_ActorReader::ReadActorDefinitionFile(std::string filepath) {
	std::ifstream file;
	file.open(filepath.c_str());

	if (!file) {
		std::cerr << "ERROR: Can't read actor file " << filepath << std::endl;
		dispErrorMessageBox(L"Can't read actor file, see console for details.");
		return;
	}

	BD_ComponentReference buffer;

	pReader* r = new pReader();
	std::string line;

	std::getline(file, line);
	int nbr_elem = std::stoi(line);

	for(int i = 0; i < nbr_elem; i++) {
		std::getline(file, line);

		r->ClearReader();
		r->parse(line);

		buffer.actor_component_index = atoi(r->GetFromKey("index").c_str());
		buffer.args = r->GetFromKey("args");
		buffer.isRoot = static_cast<bool>(atoi(r->GetFromKey("isRoot").c_str()));
		buffer.label = r->GetFromKey("label");

		buffer.loc_pos = vec3f(std::stof(r->GetFromKey("x_pos")), std::stof(r->GetFromKey("y_pos")), std::stof(r->GetFromKey("z_pos")));
		buffer.loc_rot = vec3f(std::stof(r->GetFromKey("x_rot")), std::stof(r->GetFromKey("y_rot")), std::stof(r->GetFromKey("z_rot")));
		buffer.loc_scale = vec3f(std::stof(r->GetFromKey("x_scale")), std::stof(r->GetFromKey("y_scale")), std::stof(r->GetFromKey("z_scale")));

		buffer.type = static_cast<BD_ComponentType>(std::stoi(r->GetFromKey("type")));

		m_comps.push_back(buffer);
	}

	file.close();

	return;
}

int BD_ActorReader::GetComponentCount() {
	return m_comps.size();
}

BD_ComponentReference BD_ActorReader::GetRootComponent() {
	BD_ComponentReference root = m_comps[0];
	return root;
}

BD_ComponentReference BD_ActorReader::GetComponentByIndex(int index) {
	if (index > m_comps.size()) {
		std::cerr << "ERROR: Index is too high. Returning void BD_ComponentReference." << std::endl;
		BD_ComponentReference voidref;
		return voidref;
	}

	return m_comps[index];
}