#pragma once
#include <EXP_Game.h>
#include <RD_Texture.h>

#include "EditorRegistry.h"
#include "AssetBrowser.h"

#include "imgui.h"
#include "FileUtils.h"

class EditorGUI
{
public:
	EditorGUI(EXP_Game* game, std::string projectPath, std::string contentPath);
	~EditorGUI();

	void RenderEditorGUI();

private:
	EXP_Game* m_game;

	EditorRegistry m_reg;

	std::string m_projectPath;
	std::string m_contentPath;

	AssetBrowser* m_asset_browser;
};

