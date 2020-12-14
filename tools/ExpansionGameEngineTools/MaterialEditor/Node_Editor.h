#pragma once

#include "imgui.h"
#include "imnodes.h"

#include <algorithm>
#include <vector>

#include <EXP_Game.h>
#include <EXP_Callbacks.h>

#include <vec3.h>

enum NodeType {
	TShaderNode,
	TShaderInput,
	TOp,
	TConstVec3,
	TConstVec4,
	TConstFloat
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

	virtual std::string Stringifize(Node_Editor* nedit) { return ""; };

protected:
	int m_id;
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

	void RenderNodes();

private:
	EXP_Game* m_game;
	EXP_KeyboardCallback* m_suppr;
	EXP_MouseButtonCallback* m_add;

	int m_currentIndex;
	int m_currentId;

	std::vector<Node*> m_nodes;
	std::vector<std::pair<int, int>> m_links;
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

	virtual std::string Stringifize(Node_Editor* nedit);

private:
	int m_index;
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

	virtual std::string Stringifize(Node_Editor* nedit, int index_id);

private:
	int m_index;
};

class Normalize : 
	public Node {
public:
	Normalize(int id, int index);
	virtual ~Normalize();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TOp; }
	virtual int GetNodeSize() { return 2; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit);

private:
	int m_index;
};

class Add :
	public Node {
public:
	Add(int id, int index);
	virtual ~Add();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TOp; }
	virtual int GetNodeSize() { return 3; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit);

private:
	int m_index;
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

	virtual std::string Stringifize(Node_Editor* nedit);

private:
	int m_index;

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

	virtual std::string Stringifize(Node_Editor* nedit);
private:
	int m_index;

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

	virtual std::string Stringifize(Node_Editor* nedit);

private:
	int m_index;

	float m_value;
};

class Multiply :
	public Node {
public:
	Multiply(int id, int index);
	~Multiply();

	virtual void render();

	virtual NodeType GetNodeType() { return NodeType::TOp; }
	virtual int GetNodeSize() { return 3; }
	virtual int GetId() { return m_id; }
	virtual int GetIndex() { return m_index; }

	virtual std::string Stringifize(Node_Editor* nedit);

private:
	int m_index;
};