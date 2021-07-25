#pragma once
#ifndef _ASSET_BROWSER_H__
#define _ASSET_BROWSER_H__

#include <string>
#include <vector>

#include "imgui.h"

#include "FileUtils.h"
#include "Process.h"
#include "EditorConf.h"
#include "EditorRegistry.h"

#include <EXP_Game.h>
#include <RD_Texture.h>

class AssetBrowser {
public:
	AssetBrowser(EXP_Game* game, std::string path, EditorConf* editConf, EditorRegistry* editorReg) :
		m_base_path(path), m_path("/"), m_game(game) {

		m_reg = editorReg;
		m_conf = editConf;

		m_folder_icon = game->GetRenderer()->GetRenderingAPI()->CreateTexture();
		m_folder_icon->LoadTexture("studio/icons/folder.png", false);

		m_mesh_icon = game->GetRenderer()->GetRenderingAPI()->CreateTexture();
		m_mesh_icon->LoadTexture("studio/icons/mesh.png", false);

		m_texture_icon = game->GetRenderer()->GetRenderingAPI()->CreateTexture();
		m_texture_icon->LoadTexture("studio/icons/texture.png", false);

		m_misc_icon = game->GetRenderer()->GetRenderingAPI()->CreateTexture();
		m_misc_icon->LoadTexture("studio/icons/misc.png", false);

		m_material_icon = game->GetRenderer()->GetRenderingAPI()->CreateTexture();
		m_material_icon->LoadTexture("studio/icons/material.png", false);

		m_material_draft_icon = game->GetRenderer()->GetRenderingAPI()->CreateTexture();
		m_material_draft_icon->LoadTexture("studio/icons/material_draft.png", false);

		m_files_folders = ListFilesAndFolders(m_base_path + m_path, {});
	}
	~AssetBrowser() {
		delete m_folder_icon;
		delete m_mesh_icon;
		delete m_texture_icon;
		delete m_misc_icon;

		for (auto sp : m_subprocesses) {
			delete sp;
		}
		m_subprocesses.clear();
	}

	void RefreshFilesFolders() {
		m_files_folders.clear();
		m_files_folders = ListFilesAndFolders(m_base_path + m_path, {});
	}

	void Render(float wwidth, float wheight, float shift = 0.0f) {
		ImGui::Begin(std::string("Assets - " + m_path + "###asset_browser").c_str());

		ImGui::Button("Import Asset...");
		ImGui::SameLine();

		if (ImGui::Button("Parent Folder")) {
			if (m_path.size() > 1) {
				m_path.pop_back();
				for (int i = m_path.size() - 1; i >= 0; i--) {
					if (m_path[i] != '/') {
						m_path.pop_back();
					}
					else {
						break;
					}
				}

				RefreshFilesFolders();
			}
		}

		ImGui::Separator();

		ImGui::BeginChild(1);

		for (auto ff : m_files_folders) {
			if (ff.second == ElemType::FOLDER) {
				bool isSelected = m_selected == ff.first ? true : false;
				if (ImGui::Selectable(std::string("##" + ff.first).c_str(), isSelected, 0, ImVec2(0, 30))) {
					if (m_selected == ff.first) {
						m_path += ff.first + "/";
						RefreshFilesFolders();

						ImGui::EndChild();
						ImGui::End();
						return;

						m_selected = "";
					}
					else {
						m_selected = ff.first;
					}
				}

				ImGui::SameLine();
				ImGui::Image((ImTextureID)m_folder_icon->GetTextureID(), ImVec2(30, 30));
				ImGui::SameLine();
				ImGui::Text(ff.first.c_str());
			}
		}

		for (auto ff : m_files_folders) {
			if (ff.second == ElemType::FILE) {
				std::string ext = GetExtension(ff.first);

				bool isSelected = m_selected == ff.first ? true : false;
				if (ImGui::Selectable(std::string("##" + ff.first).c_str(), isSelected, 0, ImVec2(0, 30))) {
					if (m_selected == ff.first) {
						if (ext == "msh") {
							EXP_StaticMesh* m = new EXP_StaticMesh(
								m_game, m_game->GetShaderByFileRef("/mat_red.exmtl"), m_base_path + m_path + ff.first,
								vec3f(), vec3f(), vec3f(1.0f, 1.0f, 1.0f), true
							);
							m->SetNameTag("staticMesh" + std::to_string(m_reg->m_meshes.size()));

							m_reg->m_meshes.push_back(m);

						}
						else {
							OpenFile(ff.first);
						}
					}
					else {
						m_selected = ff.first;
					}
				}

				ImGui::SameLine();
				if (ext == "msh") {
					ImGui::Image((ImTextureID)m_mesh_icon->GetTextureID(), ImVec2(30, 30));
				}
				else if ((ext == "png") || (ext == "jpg") || (ext == "bmp")) {
					ImGui::Image((ImTextureID)m_texture_icon->GetTextureID(), ImVec2(30, 30));
				}
				else if (ext == "exmtl") {
					ImGui::Image((ImTextureID)m_material_icon->GetTextureID(), ImVec2(30, 30));
				}
				else if (ext == "draft") {
					ImGui::Image((ImTextureID)m_material_draft_icon->GetTextureID(), ImVec2(30, 30));
				}
				else {
					ImGui::Image((ImTextureID)m_misc_icon->GetTextureID(), ImVec2(30, 30));
				}

				ImGui::SameLine();
				ImGui::Text(ff.first.c_str());
			}
		}

		ImGui::EndChild();
		ImGui::End();
	}

	void OpenFile(std::string file) {
		std::string ext = GetExtension(file);
		if (ext == "draft") {
			Process* p = new Process();
			p->LaunchProcess(
				std::string(m_conf->MaterialEditorPath + "MaterialEditor.exe -p ")
				+ m_base_path
				+ " -c \"\" -o "
				+ m_path + file,
				m_conf->MaterialEditorPath
			);

			m_subprocesses.push_back(p);
		}
	}

private:
	EXP_Game* m_game;
	EditorConf* m_conf;
	EditorRegistry* m_reg;

	std::string m_path;
	std::string m_base_path;

	std::vector<std::pair<std::string, ElemType>> m_files_folders;
	std::string m_selected;

	std::vector<Process*> m_subprocesses;

	RD_Texture* m_folder_icon;
	RD_Texture* m_mesh_icon;
	RD_Texture* m_texture_icon;
	RD_Texture* m_misc_icon;
	RD_Texture* m_material_icon;
	RD_Texture* m_material_draft_icon;
};

#endif //_ASSET_BROWSER_H__