#pragma once
#ifndef _ASSET_BROWSER_H__
#define _ASSET_BROWSER_H__

#include <string>
#include <vector>
#include <filesystem>

#include "imgui.h"

#include "FileUtils.h"
#include "Filebrowser.h"
#include "Process.h"
#include "EditorConf.h"
#include "EditorRegistry.h"

#include <EXP_Game.h>
#include <RD_Texture.h>

class AssetBrowser {
public:
	AssetBrowser(EXP_Game* game, std::string path, EditorConf* editConf, EditorRegistry* editorReg, EXP_MapLoader* mloader) :
		m_base_path(path), m_path("/"), m_game(game) {

		m_reg = editorReg;
		m_conf = editConf;
		m_loader = mloader;

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

#ifdef _WIN32
		m_asset_browser = new Filebrowser("C:/");
#else
		m_asset_browser = new Filebrowser("/");
#endif //_WIN32

		m_folder_create_mode = false;
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
		m_asset_browser->Render(m_game->GetRenderer());

		ImGui::Begin(std::string("Assets - " + m_path + "###asset_browser").c_str());

		if (ImGui::Button("Import Asset...")) {
			m_asset_browser->Open();
		}

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

		ImGui::SameLine();

		if (ImGui::Button("Create New Folder")) {
			m_folder_create_mode = true;
			memset(m_new_folder_name, 0, 300);
		}

		ImGui::Separator();

		ImGui::BeginChild(1);

		if (m_folder_create_mode) {
			ImGui::Image((ImTextureID)m_folder_icon->GetTextureID(), ImVec2(30, 30));
			ImGui::SameLine();
			ImVec2 cursor = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(cursor.x, cursor.y + 6.0f));

			ImGui::InputText("Folder name", m_new_folder_name, 300);
		}

		if (m_folder_create_mode && m_game->GetInputHandler()->GetKey(257)) { //Key Enter
			if (!std::filesystem::exists(m_base_path + m_path + std::string(m_new_folder_name))) {
				std::filesystem::create_directory(m_base_path + m_path + std::string(m_new_folder_name));
				RefreshFilesFolders();

				m_folder_create_mode = false;

				std::cout << "Created folder \"" << m_new_folder_name << "\"" << std::endl;
			}
			else {
				dispErrorMessageBox(StrToWStr("\"" + std::string(m_new_folder_name) + "\" already exists."));
			}
		}
		else if (m_folder_create_mode && m_game->GetInputHandler()->GetKey(256)) { //Escape Key
			m_folder_create_mode = false;
		}

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
				ImVec2 cursor = ImGui::GetCursorPos();
				ImGui::SetCursorPos(ImVec2(cursor.x, cursor.y + 6.0f));
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
								m_game,
								m_game->GetRenderer()->FetchShaderFromFile(m_game->GetRenderer()->GetEngineDir() + "/Materials/default_mat.exmtl"),
								m_base_path + m_path + ff.first,
								vec3f(), vec3f(), vec3f(1.0f, 1.0f, 1.0f), true
							);
							m->SetNameTag("staticMesh" + std::to_string(m_reg->m_meshes.size()));

							m_reg->m_meshes.push_back(std::pair<EXP_StaticMesh*, MeshMeta>(m, { "Default Material", m_path + ff.first}));

						}
						else if (ext == "json") {
							OpenMap(m_base_path + m_path + ff.first);
							m_reg->mapPath = m_base_path + m_path + ff.first;
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
				ImVec2 cursor = ImGui::GetCursorPos();
				ImGui::SetCursorPos(ImVec2(cursor.x, cursor.y + 6.0f));
				ImGui::Text(ff.first.c_str());
			}
		}

		if (ImGui::IsWindowHovered()) {
			if (m_game->GetInputHandler()->GetKey(261)) {
				if (m_selected != "") {
					std::cout << "Deleting " << m_base_path + m_path + m_selected << std::endl;
					std::filesystem::remove_all(m_base_path + m_path + m_selected);

					m_selected.clear();
					RefreshFilesFolders();
				}
			}
		}

		ImGui::EndChild();
		ImGui::End();

		if (m_asset_browser->OkPressed()) {
			if (std::filesystem::exists(m_asset_browser->GetFileNameBuffer())) {
				std::filesystem::copy(m_asset_browser->GetFileNameBuffer(), m_base_path + m_path + m_asset_browser->GetFileName());

				RefreshFilesFolders();
			}
			else {
				dispErrorMessageBox(StrToWStr(m_asset_browser->GetFileNameBuffer() + " does not exists."));
			}
			m_asset_browser->ResetBools();
		}
	}

	void RenderDeletePopup() {

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

	void OpenMap(std::string file) {
		m_loader->ClearAll();
		m_game->GetRenderer()->GetMaterialLibrary()->ClearLibrary();
		m_game->GetRenderer()->UnregisterAllDirLights();
		m_game->GetRenderer()->UnregisterAllMeshes();
		m_game->GetRenderer()->UnregisterAllPointLights();

		m_reg->m_actors.clear();
		m_reg->m_meshes.clear();
		m_reg->m_plights.clear();
		m_reg->m_dlights.clear();

		m_loader->LoadMap(file, m_base_path, true);

		std::vector<std::pair<EXP_StaticMesh*, MeshMeta>> meshes;
		for (auto m : m_loader->GetMeshList()) {
			std::string material_name = m->GetMaterial()->GetMetaInf();

			meshes.push_back(std::pair<EXP_StaticMesh*, MeshMeta>(m, { material_name, m->GetMetaInf() }));
		}
		m_reg->m_meshes = meshes;
		m_reg->m_dlights = m_loader->GetDirLightList();
		m_reg->m_plights = m_loader->GetPointLightList();

		m_reg->levelCodeObjectName = m_loader->GetLevelCodeObjectName();
	}

private:
	EXP_Game* m_game;
	EditorConf* m_conf;
	EditorRegistry* m_reg;
	EXP_MapLoader* m_loader;

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

	Filebrowser* m_asset_browser;

	bool m_folder_create_mode;
	bool m_delete_popup;

	char m_new_folder_name[300];
};

#endif //_ASSET_BROWSER_H__