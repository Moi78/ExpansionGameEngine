#pragma once

#include "imgui.h"
#include "imnodes.h"

#include <algorithm>
#include <vector>

#include <EXP_Game.h>
#include <EXP_Callbacks.h>

#include <BD_MatRW.h>

#include <vec3.h>

enum NodeType {
	TShaderNode,
	TShaderInput,
	TNormalize,
	TAdd,
	TSub,
	TMul,
	TConstVec3,
	TConstVec4,
	TConstFloat,
	TSampler2D,
	TShadowProcess,
	TNormalProcess
};

class Node_Editor;

class Node {
public:
	Node(int id) { m_id = id; }
	virtual ~Node() {};

	virtual void render() = 0;

	virtual NodeType GetNodeType() = 0;
	virtual int GetNodeSize() = 0;
	virtual int GetId() = 0;
	virtual int GetIndex() = 0;

	virtual std::string Stringifize(Node_Editor* nedit, int start_id) { return ""; };

protected:
	int m_id;
	int m_index;
};

class Node_Editor
{
public:
	Node_Editor(EXP_Game* game);
	~Node_Editor();

	void AddNode(Node* n);
	void AddNodeCallback();

	void MakeLinks();
	void DeleteLink();
	bool IsAlreadyLinked(int id);

	Node* GetNodeLinkedTo(int id_end);
	int GetLinkStartId(int end_id);

	std::string EvalNodes();

	int GetTextureCount();
	std::pair<std::string, std::string> GetTextureRefByIndex(int index);

	void SaveFinalMaterial(std::string path);
	void SaveMaterialDraft(std::string path);
	void OpenMaterialDraft(std::string path);

	void RenderNodes();

private:
	EXP_Game* m_game;
	EXP_KeyboardCallback* m_suppr;
	EXP_MouseButtonCallback* m_add;

	int m_currentIndex;
	int m_currentId;

	std::vector<Node*> m_nodes;
	std::vector<std::pair<int, int>> m_links;
	std::vector<std::pair<std::string, std::string>> m_textures;
};

class ShaderNode :
	public Node {
public:
	ShaderNode(int id, int index);
	virtual ~ShaderNode();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TShaderNode; }
	virtual int GetNodeSize() { return 8; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);
};

class ShaderInputs :
	public Node {
public:
	ShaderInputs(int id, int index);
	virtual ~ShaderInputs();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TShaderInput; }
	virtual int GetNodeSize() { return 5; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);
};

class Normalize : 
	public Node {
public:
	Normalize(int id, int index);
	virtual ~Normalize();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TNormalize; }
	virtual int GetNodeSize() { return 2; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);
};

class Add :
	public Node {
public:
	Add(int id, int index);
	virtual ~Add();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TAdd; }
	virtual int GetNodeSize() { return 3; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);
};

class ConstVec3 :
	public Node {
public:
	ConstVec3(int id, int index);
	virtual ~ConstVec3();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TConstVec3; }
	virtual int GetNodeSize() { return 1; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);

private:
	vec3f m_value;
};

class ConstVec4 :
	public Node {
public:
	ConstVec4(int id, int index);
	virtual ~ConstVec4();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TConstVec4; }
	virtual int GetNodeSize() { return 1; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);
private:
	vec4f m_value;
};

class ConstFloat :
	public Node {
public:
	ConstFloat(int id, int index);
	~ConstFloat();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TConstFloat; }
	virtual int GetNodeSize() { return 1; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);

private:
	float m_value;
};

class Multiply :
	public Node {
public:
	Multiply(int id, int index);
	~Multiply();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TMul; }
	virtual int GetNodeSize() { return 3; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);
};

class TextureSampler :
	public Node {
public:
	TextureSampler(int id, int index);
	~TextureSampler();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TSampler2D; }
	virtual int GetNodeSize() { return 7; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);

	std::string GetTexPath() { return std::string(m_tex_path); }

private:
	char m_tex_path[300];
};

class Subtract :
	public Node {
public:
	Subtract(int id, int index);
	virtual ~Subtract();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TSub; }
	virtual int GetNodeSize() { return 3; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);
};

class ProcessShadows :
	public Node {
public:
	ProcessShadows(int id, int index);
	~ProcessShadows();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TShadowProcess; }
	virtual int GetNodeSize() { return 1; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);
};

class NormalFromMap :
	public Node {
public:
	NormalFromMap(int id, int index);
	virtual ~NormalFromMap();

	virtual void render();
	
	virtual NodeType GetNodeType() { return NodeType::TNormalProcess; }
	virtual int GetNodeSize() { return 5; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit, int start_id);
};