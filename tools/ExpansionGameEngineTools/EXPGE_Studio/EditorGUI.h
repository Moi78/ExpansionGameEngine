#pragma once
#include <EXP_Game.h>
#include <EXP_InputHandler.h>
#include <RD_Texture.h>
#include <EXP_Component.h>
#include <EXP_PointLight.h>

#include "EditorRegistry.h"
#include "AssetBrowser.h"
#include "EditorConf.h"
#include "Filebrowser.h"

#include "imgui.h"
#include "FileUtils.h"

enum COMP_TYPES {
	TNONE,
	TPLIGHT,
	TDLIGHT,
	TSMESH
};

class EditorGUI
{
public:
	EditorGUI(EXP_Game* game, std::string projectPath, std::string contentPath);
	~EditorGUI();

	void RenderEditorGUI();
	void RenderMenuBar();

	//Detail Panel
	void DetailPLight(EXP_PointLight* plight);
	void DetailDLight(EXP_DirLight* dlight);
	void DetailSMesh(EXP_StaticMesh* smesh, std::string mat);

	EditorConf* GetConf() { return &m_conf; }

private:
	EXP_Game* m_game;

	EditorRegistry m_reg;
	EditorConf m_conf;

	std::pair<COMP_TYPES, void*> m_selected;
	int m_selected_index;

	std::string m_projectPath;
	std::string m_contentPath;

	AssetBrowser* m_asset_browser;
	Filebrowser* m_material_browser;
};

