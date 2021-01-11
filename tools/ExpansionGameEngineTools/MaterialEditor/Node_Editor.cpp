#include "Node_Editor.h"
#include <algorithm>

Node_Editor::Node_Editor(EXP_Game* game, std::string projectRoot, std::string contentPath) {
	m_game = game;
	m_suppr = new EXP_KeyboardCallback(game, CL_VDFUNCPTR(Node_Editor::DeleteLink), GLFW_KEY_DELETE, true);
	m_add = new EXP_MouseButtonCallback(game, CL_VDFUNCPTR(Node_Editor::AddNodeCallback), GLFW_MOUSE_BUTTON_RIGHT, false);

	m_projectRoot = projectRoot;
	m_contentPath = contentPath;
	
	m_currentIndex = 13;
	m_currentId = 1;
}

Node_Editor::~Node_Editor() {
	for (auto n : m_nodes) {
		delete n;
	}
	m_nodes.clear();
}

void Node_Editor::SaveFinalMaterial(std::string matPath) {
	BD_MatCustomShaderWrite* mw = new BD_MatCustomShaderWrite();

	m_textures.clear();
	m_contentPath.erase(m_contentPath.begin());
	for(auto* n : m_nodes)
	{
		if (n->GetNodeType() == NodeType::TSampler2D) {
			TextureSampler* s = reinterpret_cast<TextureSampler*>(n);
			m_textures.push_back(std::pair<std::string, std::string>(m_contentPath + s->GetTexPath(), "tex" + std::to_string(s->GetId())));
		}
	}
	m_contentPath = "/" + m_contentPath;
	
	for (auto tex : m_textures) {
		mw->AddTextureRef(tex.first, tex.second);
	}

	mw->SetShaderCode(EvalNodes());

	mw->WriteMaterialFile(matPath);

	delete mw;
}

void Node_Editor::SaveMaterialDraft(std::string path) {
	std::ofstream file;
	file.open(path, std::ios::binary);
	if (!file) {
		std::cerr << "ERROR: Cannot create file " << path << "." << std::endl;
		dispErrorMessageBox(StrToWStr("ERROR: Cannot create file " + path + "."));
		return;
	}

	// 1 - Nombre de nodes (int)
	// 2 - Nombre de links (int)
	// 3 - Nodes (n --> Nodes)
	// 4 - Links (2n --> int, int)

	int nbrNodes = m_nodes.size();
	int nbrLinks = m_links.size();

	file.write(reinterpret_cast<const char*>(&nbrNodes), sizeof(int));
	file.write(reinterpret_cast<const char*>(&nbrLinks), sizeof(int));

	for (auto* n : m_nodes) {
		NodeType nt = n->GetNodeType();
		file.write(reinterpret_cast<char*>(&nt), sizeof(NodeType));
		int id = n->GetId();
		file.write(reinterpret_cast<char*>(&id), sizeof(int));
		int index = n->GetIndex();
		file.write(reinterpret_cast<char*>(&index), sizeof(int));

		ImVec2 p = imnodes::GetNodeGridSpacePos(n->GetId());
		file.write(reinterpret_cast<const char*>(&p), sizeof(ImVec2));

		n->WriteNodeData(&file);
	}

	for (auto l : m_links) {
		file.write(reinterpret_cast<const char*>(&l.first), sizeof(int));
		file.write(reinterpret_cast<const char*>(&l.second), sizeof(int));
	}

	file.close();
	std::cout << "Wrote " << path << std::endl;
}

void Node_Editor::OpenMaterialDraft(std::string path) {
	std::ifstream file;
	file.open(path, std::ios::binary);
	if (!file) {
		std::cerr << "Cannot open file" << std::endl;
		return;
	}

	int nbrNodes = 0;
	int nbrLinks = 0;

	file.read(reinterpret_cast<char*>(&nbrNodes), sizeof(int));
	file.read(reinterpret_cast<char*>(&nbrLinks), sizeof(int));

	for (auto* n : m_nodes) {
		delete n;
	}
	m_nodes.clear();
	m_links.clear();

	for (int i = 0; i < nbrNodes; i++) {
		NodeType t = NodeType::TNormalize;
		file.read(reinterpret_cast<char*>(&t), sizeof(NodeType));

		int id = 0;
		int index = 0;
		file.read(reinterpret_cast<char*>(&id), sizeof(int));
		file.read(reinterpret_cast<char*>(&index), sizeof(int));

		ImVec2 pos(0.0f, 0.0f);
		file.read(reinterpret_cast<char*>(&pos), sizeof(ImVec2));

		switch (t)
		{
		case TShaderNode:
			AddNode(new ShaderNode(id, index));
			break;
		case TShaderInput:
			AddNode(new ShaderInputs(id, index));
			break;
		case TNormalize:
			AddNode(new Normalize(id, index));
			break;
		case TAdd:
			AddNode(new Add(id, index));
			break;
		case TSub:
			AddNode(new Subtract(id, index));
			break;
		case TMul:
			AddNode(new Multiply(id, index));
			break;
		case TConstVec3:
			InitConstVec3(&file, id, index);
			break;
		case TConstVec4:
			InitConstVec4(&file, id, index);
			break;
		case TConstFloat:
			InitConstFloat(&file, id, index);
			break;
		case TSampler2D:
			InitTextureSampler(&file, id, index);
			break;
		case TShadowProcess:
			AddNode(new ProcessShadows(id, index));
			break;
		case TNormalProcess:
			AddNode(new NormalFromMap(id, index));
			break;
		default:
			break;
		}

		imnodes::SetNodeEditorSpacePos(id, pos);

		m_currentId = m_nodes.back()->GetId() + 1;
		m_currentIndex = m_nodes.back()->GetIndex() + m_nodes.back()->GetNodeSize();
	}

	for (int i = 0; i < nbrLinks; i++) {
		int ls = 0; //Link Start
		int le = 0; //Link Stop

		file.read(reinterpret_cast<char*>(&ls), sizeof(int));
		file.read(reinterpret_cast<char*>(&le), sizeof(int));

		m_links.push_back(std::pair<int, int>(ls, le));
	}

	file.close();
}

void Node_Editor::RenderNodes() {
	for (auto n : m_nodes) {
		n->render();
	}

	int lid = 0;
	for (auto l : m_links) {
		imnodes::Link(lid, l.first, l.second);

		lid++;
	}
}

void Node_Editor::AddNode(Node* n) {
	m_currentIndex += n->GetNodeSize();
	m_nodes.push_back(n);
	m_currentId++;
}

void Node_Editor::MakeLinks() {
	int ls, le;
	if (imnodes::IsLinkCreated(&ls, &le)) {
		Node* a = GetNodeFromPin(ls);
		Node* b = GetNodeFromPin(le);

		//std::cout << "Node a : " << a->GetNodeType() << " Node b : " << b->GetNodeType() << std::endl;
		
		if((a->GetPinType(ls) != b->GetPinType(le)) && (a->GetPinType(ls) != PinType::TAny) && (b->GetPinType(le) != PinType::TAny)) {
			return;
		}
		
		for (auto l : m_links) {
			if ((le == l.second)) {
				return;
			}
		}
		m_links.push_back(std::pair<int, int>(ls, le));
	}
}

bool Node_Editor::IsAlreadyLinked(int id) {
	for (auto l : m_links) {
		if (l.second == id) {
			return true;
		}
	}

	return false;
}

void Node_Editor::DeleteLink() {
	int numselected = imnodes::NumSelectedLinks();

	if (numselected > 0) {
		std::vector<int> links;
		links.resize(numselected);
		imnodes::GetSelectedLinks(links.data());

		std::sort(links.begin(), links.end());

		for (int i = numselected - 1; i >= 0; i--) {
			m_links.erase(m_links.begin() + links[i]);
		}
		
		imnodes::ClearLinkSelection();
	}

	numselected = imnodes::NumSelectedNodes();
	if (numselected > 0) {
		std::vector<int> nodesid;
		nodesid.resize(numselected);
		imnodes::GetSelectedNodes(nodesid.data());

		//MUST REFACTOR
		for (auto id : nodesid) {
			if (!(id == 0 || id == 1)) {
				int i = 0;
				for (auto node : m_nodes) {

					if (node->GetId() == id) {
						for (int ind = node->GetIndex(); ind < node->GetIndex() + node->GetNodeSize(); ind++) {

							for (int a = m_links.size() - 1; a >= 0; a--) {
								if ((ind == m_links[a].first) || (ind == m_links[a].second)) {
									m_links.erase(m_links.begin() + a);
								}
							}
						}

						if (node->GetNodeType() == NodeType::TSampler2D) {
							std::string texid = "tex" + std::to_string(node->GetId());
							for (int tex = 0; tex < m_textures.size(); tex++) {
								if (m_textures[tex].second == texid) {
									m_textures.erase(m_textures.begin() + tex);
								}
							}
						}
						
						m_nodes.erase(m_nodes.begin() + i);
						break;
					}

					i++;
				}
			}
		}

		imnodes::ClearNodeSelection();
	}
}

Node* Node_Editor::GetNodeLinkedTo(const int id_end) {
	int start_id = GetLinkStartId(id_end);

	if (start_id == -1) {
		return nullptr;
	}

	for (auto n : m_nodes) {
		for (int i = n->GetIndex(); i < n->GetIndex() + n->GetNodeSize(); i++) {
			if (start_id == i) {
				return n;
			}
		}
	}

	return nullptr;
}

Node* Node_Editor::GetNodeFromPin(const int id) {
	for (auto n : m_nodes) {
		for (int i = n->GetIndex(); i < n->GetIndex() + n->GetNodeSize(); i++) {
			if (id == i) {
				return n;
			}
		}
	}

	return nullptr;
}


int Node_Editor::GetLinkStartId(int end_id) {
	int start_id = -1;
	for (auto l : m_links) {
		if (l.second == end_id) {
			start_id = l.first;
		}
	}

	return start_id;
}

void Node_Editor::AddNodeCallback() {
	ImGui::OpenPopup("MenuAdd");

	if (ImGui::BeginPopup("MenuAdd")) {
		ImGui::Text("Add Node");
		ImGui::Separator();

		ImVec2 pos = ImGui::GetMousePosOnOpeningCurrentPopup();

		if (ImGui::MenuItem("Add")) {
			AddNode(new Add(m_currentId + 1, m_currentIndex));
			imnodes::SetNodeScreenSpacePos(m_currentId, pos);
		}

		if (ImGui::MenuItem("Constant Float")) {
			AddNode(new ConstFloat(m_currentId + 1, m_currentIndex));
			imnodes::SetNodeScreenSpacePos(m_currentId, pos);
		}

		if (ImGui::MenuItem("Constant Vec 3")) {
			AddNode(new ConstVec3(m_currentId + 1, m_currentIndex));
			imnodes::SetNodeScreenSpacePos(m_currentId, pos);
		}

		if (ImGui::MenuItem("Constant Vec 4")) {
			AddNode(new ConstVec4(m_currentId + 1, m_currentIndex));
			imnodes::SetNodeScreenSpacePos(m_currentId, pos);
		}

		if (ImGui::MenuItem("Multiply")) {
			AddNode(new Multiply(m_currentId + 1, m_currentIndex));
			imnodes::SetNodeScreenSpacePos(m_currentId, pos);
		}

		if (ImGui::MenuItem("NormalFromMap")) {
			AddNode(new NormalFromMap(m_currentId + 1, m_currentIndex));
			imnodes::SetNodeScreenSpacePos(m_currentId, pos);
		}

		if (ImGui::MenuItem("Normalize")) {
			AddNode(new Normalize(m_currentId + 1, m_currentIndex));
			imnodes::SetNodeScreenSpacePos(m_currentId, pos);
		}

		if (ImGui::MenuItem("Subtract")) {
			AddNode(new Subtract(m_currentId + 1, m_currentIndex));
			imnodes::SetNodeScreenSpacePos(m_currentId, pos);
		}

		if (ImGui::MenuItem("TextureSampler")) {
			AddNode(new TextureSampler(m_currentId + 1, m_currentIndex));
			imnodes::SetNodeScreenSpacePos(m_currentId, pos);
		}

		ImGui::EndPopup();
	}
}

std::string Node_Editor::EvalNodes() {
	std::string outCode;

	//Clearing old trash
	m_textures.clear();

	//Copying Fragment Shader Head
	std::ifstream head;
	head.open("Engine/ShaderFragments/FragHead_PBR.txt", std::ios::beg);
	if (!head) {
		std::cerr << "Cannot open fraghead" << std::endl;
		return "";
	}

	while (!head.eof()) {
		char line[100];
		head.getline(line, 100);
		outCode += line;
		outCode += "\n";
	}

	//Pre-Scan of nodes
	bool linkedShadowProcess = false;
	bool linkedNormalMapProcess = false;
	for (auto n : m_nodes) {
		if (n->GetNodeType() == NodeType::TSampler2D) {
			outCode += "uniform sampler2D tex" + std::to_string(n->GetId()) + ";\n";

			TextureSampler* s = reinterpret_cast<TextureSampler*>(n);
			m_textures.push_back(std::pair<std::string, std::string>(m_projectRoot + m_contentPath + s->GetTexPath(), "tex" + std::to_string(s->GetId())));
		}

		if (n->GetNodeType() == NodeType::TNormalProcess && (!linkedNormalMapProcess)) {
			std::ifstream bumpFunc;
			bumpFunc.open("Engine/ShaderFragments/BumpMap.txt", std::ios::beg);
			if (!bumpFunc) {
				return "";
			}

			while (!bumpFunc.eof()) {
				char line[200];
				bumpFunc.getline(line, 200);
				outCode += std::string(line) + "\n";
			}

			linkedNormalMapProcess = true;
		}
	}

	outCode += "void main() {\n";
	
	outCode += m_nodes[0]->Stringifize(this, 0);

	outCode += "}";

	std::cout << outCode << std::endl;

	head.close();

	return outCode;
}

int Node_Editor::GetTextureCount() const {
	return m_textures.size();
}

std::pair<std::string, std::string> Node_Editor::GetTextureRefByIndex(int index) {
	assert(index < GetTextureCount() && "Given index too high");

	return m_textures[index];
}

ShaderNode::ShaderNode(int id, int index) : Node(id) {
	m_index = index;
}

ShaderNode::~ShaderNode() {}

void ShaderNode::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(5, 156, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(3, 100, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(3, 100, 0, 255));

	ImGui::PushItemWidth(150.0f);

	imnodes::BeginNode(m_id);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("Shader Output");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC3);
	
	imnodes::BeginInputAttribute(m_index + 0);
	ImGui::Text("Base Color");
	imnodes::EndInputAttribute();

	imnodes::BeginInputAttribute(m_index + 1);
	ImGui::Text("Position");
	imnodes::EndInputAttribute();

	imnodes::BeginInputAttribute(m_index + 2);
	ImGui::Text("Normal");
	imnodes::EndInputAttribute();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_FLOAT);
	
	imnodes::BeginInputAttribute(m_index + 3);
	ImGui::Text("Specular");
	imnodes::EndInputAttribute();

	imnodes::BeginInputAttribute(m_index + 4);
	ImGui::Text("Roughness");
	imnodes::EndInputAttribute();

	imnodes::BeginInputAttribute(m_index + 5);
	ImGui::Text("Metallic");
	imnodes::EndInputAttribute();

	imnodes::BeginInputAttribute(m_index + 6);
	ImGui::Text("Ambient Occlusion");
	imnodes::EndInputAttribute();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC3);
	
	imnodes::BeginInputAttribute(m_index + 7);
	ImGui::Text("Emissive Color");
	imnodes::EndInputAttribute();

	imnodes::EndNode();
}

std::string ShaderNode::Stringifize(Node_Editor* nedit, int start_id) {
	std::string outCode;

	//Albedo
	Node* nColor = nedit->GetNodeLinkedTo(m_index + 0);
	if (nColor) {
		outCode += "gAlbedo = vec4(" + nColor->Stringifize(nedit, nedit->GetLinkStartId(m_index + 0)) + ", 0.0);\n";
	}
	else {
		outCode += "gAlbedo = vec4(0.0, 0.0, 0.0, 0.0);\n";
	}

	//Pos
	Node* nPos = nedit->GetNodeLinkedTo(m_index + 1);
	if (nPos) {
		outCode += "gPos = " + nPos->Stringifize(nedit, nedit->GetLinkStartId(m_index + 1)) + ";\n";
	}
	else {
		outCode += "gPos = vec3(0.0, 0.0, 0.0);\n";
	}

	//Norm
	Node* nNorm = nedit->GetNodeLinkedTo(m_index + 2);
	if (nNorm) {
		outCode += "gNorm = " + nNorm->Stringifize(nedit, nedit->GetLinkStartId(m_index + 2)) + ";\n";
	}
	else {
		outCode += "gNorm = vec3(0.0, 0.0, 0.0);\n";
	}

	//Specular
	Node* nSpec = nedit->GetNodeLinkedTo(m_index + 3);
	if (nSpec) {
		outCode += "gSpec = " + nSpec->Stringifize(nedit, nedit->GetLinkStartId(m_index + 3)) + ";\n";
	}
	else {
		outCode += "gSpec = 0.0;\n";
	}

	//Metallic & Roughness & AO
	Node* nMet = nedit->GetNodeLinkedTo(m_index + 5);
	Node* nRoughness = nedit->GetNodeLinkedTo(m_index + 4);
	Node* nAO = nedit->GetNodeLinkedTo(m_index + 6);
	outCode += "gMetRoughAO = vec3(";

	if (nMet) {
		outCode += nMet->Stringifize(nedit, nedit->GetLinkStartId(m_index + 5));
	}
	else {
		outCode += "0.0";
	}

	outCode += ",";

	if (nRoughness) {
		outCode += nRoughness->Stringifize(nedit, nedit->GetLinkStartId(m_index + 4));
	}
	else {
		outCode += "0.0";
	}

	outCode += ",";

	if (nAO) {
		outCode += nAO->Stringifize(nedit, nedit->GetLinkStartId(m_index + 6));
	}
	else {
		outCode += "0.0";
	}

	outCode += ");\n";

	//Emissive
	Node* emissive = nedit->GetNodeLinkedTo(m_index + 7);
	if(emissive) {
		outCode += "gEmissive = " + emissive->Stringifize(nedit, nedit->GetLinkStartId(m_index + 7)) + ";\n";
	}  else {
		outCode += "gEmissive = vec3(0.0, 0.0, 0.0);\n";
	}

	return outCode;
}

ShaderInputs::ShaderInputs(int id, int index) : Node(id) {
	m_index = index;
}

ShaderInputs::~ShaderInputs() {

}

void ShaderInputs::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(255, 0, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(100, 0, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(100, 0, 0, 255));

	imnodes::BeginNode(m_id);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("Shader Inputs");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC3);
	
	imnodes::BeginOutputAttribute(m_index + 0);
	ImGui::Text("Normal");
	imnodes::EndOutputAttribute();

	imnodes::BeginOutputAttribute(m_index + 1);
	ImGui::Text("Tangent");
	imnodes::EndOutputAttribute();

	imnodes::BeginOutputAttribute(m_index + 2);
	ImGui::Text("Bitangent");
	imnodes::EndOutputAttribute();
	
	imnodes::BeginOutputAttribute(m_index + 3);
	ImGui::Text("Fragment Position");
	imnodes::EndOutputAttribute();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC2);
	
	imnodes::BeginOutputAttribute(m_index + 4);
	ImGui::Text("UV Coords");
	imnodes::EndOutputAttribute();

	imnodes::EndNode();
}

std::string ShaderInputs::Stringifize(Node_Editor* nedit, int index_id) {
	//std::cout << "Index ID : " << index_id << " Relative ID : " << index_id - m_index << std::endl;
	switch (index_id - m_index) {
	case 0:
		return "Normal";
		break;
	case 1:
		return "Tan";
		break;
	case 2:
		return "Bitan";
		break;
	case 3:
		return "FragPos";
		break;
	case 4:
		return "UVcoord";
		break;
	default:
		return "";
		break;
	}
}

Normalize::Normalize(int id, int index) : Node(id){
	m_index = index;
}

Normalize::~Normalize() {

}

void Normalize::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(255, 119, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(200, 100, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(200, 100, 0, 255));

	imnodes::BeginNode(m_id);
	ImGui::PushItemWidth(150.0f);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("Normalize");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_ANY);
	
	imnodes::BeginInputAttribute(m_index + 0);
	ImGui::Text("Vector");
	imnodes::EndInputAttribute();

	imnodes::BeginOutputAttribute(m_index + 1);
	ImGui::Text("Normalized Vector");
	imnodes::EndInputAttribute();

	imnodes::EndNode();
}

std::string Normalize::Stringifize(Node_Editor* nedit, int start_id) {
	std::string outCode = "";

	Node* vec = nedit->GetNodeLinkedTo(m_index + 0);
	if (vec) {
		outCode += "normalize(" + vec->Stringifize(nedit, nedit->GetLinkStartId(m_index + 0)) + ")";
	}
	else {
		outCode += "normalize(vec3(0.0, 0.0, 0.0))";
	}

	return outCode;
}

Add::Add(int id, int index) : Node(id) {
	m_index = index;
}

Add::~Add() {

}

void Add::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(255, 119, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(200, 100, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(200, 100, 0, 255));

	imnodes::BeginNode(m_id);

	ImGui::PushItemWidth(100.0f);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("Add");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_ANY);
	
	imnodes::BeginInputAttribute(m_index + 0);
	ImGui::Text("a");
	imnodes::EndInputAttribute();

	imnodes::BeginInputAttribute(m_index + 1);
	ImGui::Text("b");
	imnodes::EndInputAttribute();

	imnodes::BeginOutputAttribute(m_index + 2);
	ImGui::Text("Result");
	imnodes::EndOutputAttribute();

	imnodes::EndNode();
}

std::string Add::Stringifize(Node_Editor* nedit, int start_id) {
	std::string outCode = "(";

	Node* a = nedit->GetNodeLinkedTo(m_index + 0);
	Node* b = nedit->GetNodeLinkedTo(m_index + 1);

	if(a) {
		outCode += a->Stringifize(nedit, nedit->GetLinkStartId(m_index + 0));
	}
	else {
		outCode += "0.0";
	}

	outCode += "+";

	if (b) {
			outCode += b->Stringifize(nedit, nedit->GetLinkStartId(m_index + 1));
	}
	else {
		outCode += "0.0";
	}

	outCode += ")";

	return outCode;
}

ConstVec3::ConstVec3(int id, int index) : Node(id) {
	m_index = index;
	m_value = vec3f();
}

ConstVec3::~ConstVec3() {}

void ConstVec3::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(std::clamp((int)(m_value.getX() * 255), 0, 255),
																   std::clamp((int)(m_value.getY() * 255), 0, 255),
																   std::clamp((int)(m_value.getZ() * 255), 0, 255), 255));

	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(std::clamp((int)(m_value.getX() * 200), 0, 255),
																		  std::clamp((int)(m_value.getY() * 200), 0, 255),
																		  std::clamp((int)(m_value.getZ() * 200), 0, 255), 255));

	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(std::clamp((int)(m_value.getX() * 200), 0, 255),
																		   std::clamp((int)(m_value.getY() * 200), 0, 255),
																		   std::clamp((int)(m_value.getZ() * 200), 0, 255), 255));

	imnodes::BeginNode(m_id);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("ConstVec3");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC3);
	
	imnodes::BeginOutputAttribute(m_index + 0);
	ImGui::InputFloat3("Vec3", m_value.GetPTR());
	imnodes::EndOutputAttribute();

	imnodes::EndNode();
}

std::string ConstVec3::Stringifize(Node_Editor* nedit, int start_id) {
	return "vec3(" + std::to_string(m_value.getX()) + "," + std::to_string(m_value.getY()) + "," + std::to_string(m_value.getZ()) + ")";
}

void ConstVec3::WriteNodeData(std::ofstream* file) {
	file->write(reinterpret_cast<char*>(&m_value), sizeof(vec3f));
}

ConstVec4::ConstVec4(int id, int index) : Node(id) {
	m_index = index;
}

ConstVec4::~ConstVec4() {

}

void ConstVec4::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(std::clamp((int)(m_value.GetX() * 255), 0, 255),
																   std::clamp((int)(m_value.GetY() * 255), 0, 255),
																   std::clamp((int)(m_value.GetZ() * 255), 0, 255), 255));

	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(std::clamp((int)(m_value.GetX() * 200), 0, 255),
																		  std::clamp((int)(m_value.GetY() * 200), 0, 255),
																		  std::clamp((int)(m_value.GetZ() * 200), 0, 255), 255));

	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(std::clamp((int)(m_value.GetX() * 200), 0, 255),
																		   std::clamp((int)(m_value.GetY() * 200), 0, 255),
																		   std::clamp((int)(m_value.GetZ() * 200), 0, 255), 255));

	imnodes::BeginNode(m_id);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("ConstVec4");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC4);
	
	imnodes::BeginOutputAttribute(m_index + 0);
	ImGui::InputFloat4("Vec4", m_value.GetPTR());
	imnodes::EndOutputAttribute();

	imnodes::EndNode();
}

std::string ConstVec4::Stringifize(Node_Editor* nedit, int start_id) {
	return "vec4(" + std::to_string(m_value.GetX()) + ","
				   + std::to_string(m_value.GetY()) + ","
				   + std::to_string(m_value.GetZ()) + ","
				   + std::to_string(m_value.GetW()) + ")";
}

void ConstVec4::WriteNodeData(std::ofstream* file) {
	file->write(reinterpret_cast<char*>(&m_value), sizeof(vec4f));
}


ConstFloat::ConstFloat(int id, int index) : Node(id) {
	m_index = index;

	m_value = 0.0f;
}

ConstFloat::~ConstFloat() {

}

void ConstFloat::render() {
	int color = std::clamp((int)(m_value * 255), 0, 255);
	int color2 = std::clamp((int)(m_value * 200), 0, 255);

	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(color, color, color, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(color2, color2, color2, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(color2, color2, color2, 255));

	imnodes::BeginNode(m_id);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("ConstFloat");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_FLOAT);
	
	imnodes::BeginOutputAttribute(m_index + 0);
	ImGui::InputFloat("Float", &m_value);
	imnodes::EndOutputAttribute();
	imnodes::EndNode();
}

std::string ConstFloat::Stringifize(Node_Editor* nedit, int start_id) {
	return "(" + std::to_string(m_value) + ")";
}

void ConstFloat::WriteNodeData(std::ofstream* file) {
	file->write(reinterpret_cast<char*>(&m_value), sizeof(float));
}

Multiply::Multiply(int id, int index) : Node(id) {
	m_index = index;
}

Multiply::~Multiply() {

}

void Multiply::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(255, 119, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(200, 100, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(200, 100, 0, 255));

	imnodes::BeginNode(m_id);
	
	imnodes::BeginNodeTitleBar();
	ImGui::Text("Multiply");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_ANY);
	
	imnodes::BeginInputAttribute(m_index + 0);
	ImGui::Text("a");
	imnodes::EndInputAttribute();

	imnodes::BeginInputAttribute(m_index + 1);
	ImGui::Text("b");
	imnodes::EndInputAttribute();

	imnodes::BeginOutputAttribute(m_index + 2);
	ImGui::Text("Result");
	imnodes::EndOutputAttribute();

	imnodes::EndNode();
}

std::string Multiply::Stringifize(Node_Editor* nedit, int start_id) {
	std::string outCode = "(";

	Node* a = nedit->GetNodeLinkedTo(m_index + 0);
	Node* b = nedit->GetNodeLinkedTo(m_index + 1);

	if (a) {
		outCode += a->Stringifize(nedit, nedit->GetLinkStartId(m_index + 0));
	}
	else {
		outCode += "0.0";
	}

	outCode += "*";

	if (b) {
		outCode += b->Stringifize(nedit, nedit->GetLinkStartId(m_index + 1));
	}
	else {
		outCode += "0.0";
	}

	outCode += ")";

	return outCode;
}

TextureSampler::TextureSampler(int id, int index) : Node(id) {
	m_index = index;
	memset(m_tex_path, 0, 300);
}

TextureSampler::~TextureSampler() {}

void TextureSampler::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(255, 119, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(200, 100, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(200, 100, 0, 255));

	imnodes::BeginNode(m_id);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("TextureSampler");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC2);
	
	imnodes::BeginInputAttribute(m_index + 0);
	ImGui::Text("UV Coords");
	imnodes::EndInputAttribute();

	ImGui::InputText("Texture Path", m_tex_path, 300);

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC4);
	
	imnodes::BeginOutputAttribute(m_index + 1);
	ImGui::Text("RGBA");
	imnodes::EndOutputAttribute();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC3);
	
	imnodes::BeginOutputAttribute(m_index + 2);
	ImGui::Text("RGB");
	imnodes::EndOutputAttribute();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_FLOAT);
	
	imnodes::BeginOutputAttribute(m_index + 3);
	ImGui::Text("R");
	imnodes::EndOutputAttribute();

	imnodes::BeginOutputAttribute(m_index + 4);
	ImGui::Text("G");
	imnodes::EndOutputAttribute();

	imnodes::BeginOutputAttribute(m_index + 5);
	ImGui::Text("B");
	imnodes::EndOutputAttribute();

	imnodes::BeginOutputAttribute(m_index + 6);
	ImGui::Text("A");
	imnodes::EndOutputAttribute();

	imnodes::EndNode();
}

std::string TextureSampler::Stringifize(Node_Editor* nedit, int start_id) {
	std::string outCode = "";

	outCode += "texture(tex" + std::to_string(m_id) + ",";

	Node* UV = nedit->GetNodeLinkedTo(m_index + 0);
	if (UV) {
		outCode += UV->Stringifize(nedit, nedit->GetLinkStartId(m_index + 0));
	}
	else {
		outCode += "UVcoord";
	}

	outCode += ")";

	switch (start_id - m_index) {
	case 1:
		outCode += ".rgba";
		break;
	case 2:
		outCode += ".rgb";
		break;
	case 3:
		outCode += ".r";
		break;
	case 4:
		outCode += ".g";
		break;
	case 5:
		outCode += ".b";
		break;
	case 6:
		outCode += ".a";
		break;
	default:
		outCode += ".rgb";
		break;
	}

	return outCode;
}

void TextureSampler::WriteNodeData(std::ofstream* file) {
	file->write(m_tex_path, 300);
}

Subtract::Subtract(int id, int index) : Node(id) {
	m_index = index;
}

Subtract::~Subtract() {

}

void Subtract::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(255, 119, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(200, 100, 0, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(200, 100, 0, 255));

	imnodes::BeginNode(m_id);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("Subtract");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_ANY);
	
	imnodes::BeginInputAttribute(m_index + 0);
	ImGui::Text("a");
	imnodes::EndInputAttribute();

	imnodes::BeginInputAttribute(m_index + 1);
	ImGui::Text("b");
	imnodes::EndInputAttribute();

	imnodes::BeginOutputAttribute(m_index + 2);
	ImGui::Text("Result");
	imnodes::EndOutputAttribute();

	imnodes::EndNode();
}

std::string Subtract::Stringifize(Node_Editor* nedit, int end_id) {
	std::string outCode = "(";

	Node* a = nedit->GetNodeLinkedTo(m_index + 0);
	Node* b = nedit->GetNodeLinkedTo(m_index + 1);

	if (a) {
		outCode += a->Stringifize(nedit, nedit->GetLinkStartId(m_index + 0));
	}
	else {
		outCode += "0.0";
	}

	outCode += "-";

	if (b) {
		outCode += b->Stringifize(nedit, nedit->GetLinkStartId(m_index + 1));
	}
	else {
		outCode += "0.0";
	}

	outCode += ")";

	return outCode;
}

ProcessShadows::ProcessShadows(int id, int index) : Node(id) {
	m_index = index;
}

ProcessShadows::~ProcessShadows() {

}

void ProcessShadows::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(0, 213, 255, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(0, 183, 219, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(0, 183, 219, 255));

	imnodes::BeginNode(m_id);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("Process Shadow");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_FLOAT);

	imnodes::BeginOutputAttribute(m_index + 0);
	ImGui::Text("Shadow Mapping Data");
	imnodes::EndOutputAttribute();

	imnodes::EndNode();
}

std::string ProcessShadows::Stringifize(Node_Editor* nedit, int start_id) {
	return "ProcessShadows()";
}

NormalFromMap::NormalFromMap(int id, int index) : Node(id) {
	m_index = index;
}

NormalFromMap::~NormalFromMap() {

}

void NormalFromMap::render() {
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(0, 213, 255, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(0, 183, 219, 255));
	imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(0, 183, 219, 255));

	imnodes::BeginNode(m_id);

	imnodes::BeginNodeTitleBar();
	ImGui::Text("NormalFromMap");
	imnodes::EndNodeTitleBar();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC3);
	
	imnodes::BeginInputAttribute(m_index + 0);
	ImGui::Text("RGB Texture");
	imnodes::EndInputAttribute();

	imnodes::BeginInputAttribute(m_index + 1);
	ImGui::Text("Normal");
	imnodes::EndInputAttribute();

	imnodes::BeginInputAttribute(m_index + 2);
	ImGui::Text("Position");
	imnodes::EndInputAttribute();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC2);
	
	imnodes::BeginInputAttribute(m_index + 3);
	ImGui::Text("UV Coords");
	imnodes::EndInputAttribute();

	imnodes::PushColorStyle(imnodes::ColorStyle_Pin, PIN_VEC3);
	
	imnodes::BeginOutputAttribute(m_index + 4);
	ImGui::Text("Normal Data");
	imnodes::EndOutputAttribute();

	imnodes::EndNode();
}

std::string NormalFromMap::Stringifize(Node_Editor* nedit, int start_id) {
	std::string outCode = "GetNormFromMap(";

	Node* nmap = nedit->GetNodeLinkedTo(m_index + 0);
	if (nmap) {
		outCode += nmap->Stringifize(nedit, nedit->GetLinkStartId(m_index + 0));
	}
	else {
		outCode += "vec3(0.0, 0.0, 0.0)";
	}

	outCode += ",";

	Node* norm = nedit->GetNodeLinkedTo(m_index + 1);
	if (norm) {
		outCode += norm->Stringifize(nedit, nedit->GetLinkStartId(m_index + 1));
	}
	else {
		outCode += "Normal";
	}

	outCode += ",";

	Node* fpos = nedit->GetNodeLinkedTo(m_index + 2);
	if (fpos) {
		outCode += norm->Stringifize(nedit, nedit->GetLinkStartId(m_index + 2));
	}
	else {
		outCode += "FragPos";
	}

	outCode += ",";

	Node* uv = nedit->GetNodeLinkedTo(m_index + 3);
	if (uv) {
		outCode += uv->Stringifize(nedit, nedit->GetLinkStartId(m_index + 3));
	}
	else {
		outCode += "UVcoord";
	}

	outCode += ")";

	return outCode;
}