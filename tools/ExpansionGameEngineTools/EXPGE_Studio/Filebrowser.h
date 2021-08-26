#pragma once

#include "imgui.h"

#include <filesystem>
#include <string>
#include <iostream>
#include "FileUtils.h"

class Filebrowser {
public:
	Filebrowser(std::string projectRoot) {
		m_isOpened = false;
		m_projectRoot = projectRoot;
		m_actualPath = "/";
		m_fileSelected = false;
		m_folderSelected = false;
		m_okPressed = false;

		m_content = ListFilesAndFolders(m_projectRoot + m_actualPath);
	}

	~Filebrowser() {

	}

	void Render(RaindropRenderer* rndr) {
		if (m_isOpened) {
			ImGui::OpenPopup("File Browser");
			ImGui::BeginPopupModal("File Browser");

			ImGui::Text(m_actualPath.c_str());

			if (ImGui::Button("Parent Folder") && m_actualPath.size() > 1) {
				m_actualPath.pop_back();
				for (int i = m_actualPath.size(); i >= 0; i--) {
					if (m_actualPath[i] == '/') {
						m_actualPath.erase(m_actualPath.begin() + i + 1, m_actualPath.end());
						m_content = ListFilesAndFolders(m_projectRoot + m_actualPath);
						break;
					}
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Create Folder")) {
				m_createFolderMode = true;
			}

			//ImGui::ListBoxHeader("Directory", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 100));
			ImGui::BeginChildFrame(3, ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 120));

			if (m_createFolderMode) {
				ImGui::InputText("New Folder", m_folderNameBuff, 300);

				ImGui::SameLine();
				if (ImGui::Button("Ok")) {
					CreateFolder(m_folderNameBuff);
					memset(m_folderNameBuff, 0, 300);
				}

				if (rndr->GetRenderingAPI()->GetWindowingSystem()->GetKeyPress(GLFW_KEY_ENTER)) {
					CreateFolder(m_folderNameBuff);
					memset(m_folderNameBuff, 0, 300);
				}
			}

			for (auto f : m_content) {
				if (f.second == ElemType::FOLDER) {
					std::string dispName = std::string("[Folder] ") + f.first;
					if (ImGui::Selectable(dispName.c_str(), f == m_selected)) {
						if (f == m_selected) {
							m_actualPath += f.first + "/";

							m_content = ListFilesAndFolders(m_projectRoot + m_actualPath);
							m_createFolderMode = false;
							break;
						}
						else {
							m_selected = f;
							m_folderSelected = true;
						}
					}
				}
			}

			for (auto f : m_content) {
				if (f.second == ElemType::FILE) {
					std::string dispName = std::string("[File]   ") + f.first;
					if (ImGui::Selectable(dispName.c_str(), f == m_selected)) {
						if (f == m_selected) {
							m_isOpened = false;
							m_fileSelected = true;
							m_okPressed = true;
							strcpy(m_fileSelectedName, f.first.c_str());
						}
						else {
							m_selected = f;
							m_fileSelected = true;
							strcpy(m_fileSelectedName, f.first.c_str());
						}
					}
				}
			}
			//ImGui::ListBoxFooter();
			ImGui::EndChildFrame();

			ImGui::PushItemWidth(ImGui::GetWindowWidth() - 140);
			ImGui::InputText("File", m_fileSelectedName, 300);

			ImGui::SameLine();
			if (ImGui::Button("Ok")) {
				m_isOpened = false;
				m_createFolderMode = false;
				m_okPressed = true;
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel")) {
				memset(m_fileSelectedName, 0, 300);
				m_isOpened = false;
				m_createFolderMode = false;
			}

			ImGui::EndPopup();

			//Handle Element deletion
			if (rndr->GetRenderingAPI()->GetWindowingSystem()->GetKeyPress(GLFW_KEY_DELETE) && (m_folderSelected || m_fileSelected)) {
				if (!std::filesystem::remove(m_projectRoot + m_actualPath + m_selected.first)) {
					if (m_selected.second == ElemType::FOLDER) {
						std::cerr << "Cannot remove folder " << m_selected.first << std::endl;
						dispErrorMessageBox(StrToWStr(std::string("Cannot remove folder ") + m_selected.first));
					}
					else {
						std::cerr << "Cannot remove file " << m_selected.first << std::endl;
						dispErrorMessageBox(StrToWStr(std::string("Cannot remove file ") + m_selected.first));
					}
				}
				else {
					m_content = ListFilesAndFolders(m_projectRoot + m_actualPath);
					memset(m_fileSelectedName, 0, 300);
					m_folderSelected = false;
					m_fileSelected = false;
				}
			}
		}
		else {
			m_okPressed = false;
		}
	}

	void Open() {
		m_isOpened = true;
		m_content = ListFilesAndFolders(m_projectRoot + m_actualPath);
	}

	std::string GetSelectedFile() {
		return m_projectRoot + m_actualPath + std::string(m_fileSelectedName);
	}

	bool IsOpened() {
		return m_isOpened;
	}

	bool HasSelected() {
		return (m_fileSelected && !m_isOpened);
	}

	void AddFilter(std::string filter) {
		m_filters.push_back(filter);

		m_content = ListFilesAndFolders(m_projectRoot + m_actualPath);
	}

	bool OkPressed() {
		return (m_okPressed && !m_isOpened);
	}

	std::string GetFileNameBuffer() {
		if (m_filters.size() != 0) {
			std::string raw_buff(m_fileSelectedName);

			std::string ext = GetExtension(raw_buff);
			for (auto f : m_filters) {
				if (ext == f) {
					return m_projectRoot + m_actualPath + raw_buff;
				}
			}

			raw_buff += ".";
			raw_buff += m_filters[0]; //Putting first filter
			return m_projectRoot + m_actualPath + raw_buff;
		}
		else {
			return m_projectRoot + m_actualPath + std::string(m_fileSelectedName);
		}
	}

	std::string GetRelativeFileName() {
		if (m_filters.size() != 0) {
			std::string raw_buff(m_fileSelectedName);

			std::string ext = GetExtension(raw_buff);
			for (auto f : m_filters) {
				if (ext == f) {
					return m_actualPath + raw_buff;
				}
			}

			raw_buff += ".";
			raw_buff += m_filters[0]; //Putting first filter
			return m_actualPath + raw_buff;
		}
		else {
			return m_actualPath + std::string(m_fileSelectedName);
		}
	}

	std::string GetFileName() {
		if (m_filters.size() != 0) {
			std::string raw_buff(m_fileSelectedName);

			std::string ext = GetExtension(raw_buff);
			for (auto f : m_filters) {
				if (ext == f) {
					return raw_buff;
				}
			}

			raw_buff += ".";
			raw_buff += m_filters[0]; //Putting first filter
			return raw_buff;
		}
		else {
			return std::string(m_fileSelectedName);
		}
	}

	void ResetBools() {
		m_isOpened = false;
		m_okPressed = false;
		m_fileSelected = false;

		memset(m_fileSelectedName, 0, 300);
	}

private:
	std::vector<std::pair<std::string, ElemType>> ListFilesAndFolders(std::string path) {
		try {
			if (std::filesystem::is_empty(path)) {
				return {};
			}
		}
		catch (std::exception& e) {
			return {};
		}

		//Iter over dir
		std::vector<std::pair<std::string, ElemType>> currentDirContent;

		for (auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.is_directory()) {
				currentDirContent.push_back(std::pair<std::string, ElemType>(entry.path().filename().string(), ElemType::FOLDER));
			}
			else {
				try {
					if (m_filters.size() > 0) {
						for (auto i : m_filters) {
							if (i == GetExtension(entry.path().filename().string()) && !entry.is_directory()) {
								currentDirContent.push_back(std::pair<std::string, ElemType>(entry.path().filename().string(), ElemType::FILE));
								break;
							}
						}
					}
					else {
						currentDirContent.push_back(std::pair<std::string, ElemType>(entry.path().filename().string(), ElemType::FILE));
					}
				}
				catch (std::exception& e) {
					/* Nothing to do as the error is probably something like "No UTF-8 Character" */
				}
			}
		}

		return currentDirContent;
	}

	std::string GetExtension(std::string name) {
		std::string buff;
		for (int i = name.size(); i >= 0; i--) {
			if (name[i] != '.') {
				buff = std::string(name.begin() + i, name.end());
			}
			else {
				break;
			}
		}

		return buff;
	}

	void CreateFolder(std::string name) {
		bool createFolder = false;

		if (name.size() == 0) {
			m_createFolderMode = false;
			createFolder = false;
		}

		for (auto c : name) {
			if ((c == '/') ||
				(c == '\\') ||
				(c == '*') ||
				(c == ':') ||
				(c == '?') ||
				(c == '\"') ||
				(c == '|') ||
				(c == '<') ||
				(c == '>'))
			{
				m_createFolderMode = false;
				createFolder = false;
			}
			else {
				m_createFolderMode = false;
				createFolder = true;
			}
		}

		if (createFolder) {
			if (!std::filesystem::create_directory(std::filesystem::path(m_projectRoot + m_actualPath + std::string(m_folderNameBuff)))) {
				std::cerr << "Cannot create folder." << std::endl;
			}
			else {
				m_content = ListFilesAndFolders(m_projectRoot + m_actualPath);
			}
		}
	}

	bool m_isOpened;
	bool m_okPressed;
	bool m_fileSelected;
	bool m_folderSelected;
	bool m_createFolderMode;

	char m_folderNameBuff[300] = { 0 };

	std::string m_projectRoot;
	std::string m_actualPath;

	std::pair<std::string, ElemType> m_selected;
	std::vector<std::pair<std::string, ElemType>> m_content;

	std::vector<std::string> m_filters;
	char m_fileSelectedName[300] = { 0 };
};