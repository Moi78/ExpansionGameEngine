#pragma once

#include <string>
#include <vector>

template<class T>
class RD_GenericRessourceManager {
public:
	RD_GenericRessourceManager() {}
	~RD_GenericRessourceManager() {
		CleanAll();
	}

	void AddRessource(T* res, std::string name) {
		m_ressources.push_back(std::pair<T*, std::string>(res, name));
	}

	T* GetRessourceByName(std::string name) {
		for (auto& res : m_ressources) {
			if (name == res.second) {
				return res.first;
			}
		}
	}

	bool DoRessourceExists(std::string name) {
		for (auto& res : m_ressources) {
			if (name == res.second) {
				return true;
			}
		}

		return false;
	}

	void CleanAll() {
		for (auto elems : m_ressources) {
			delete elems.first;
		}
		m_ressources.clear();
	}

private:
	std::vector<std::pair<T*, std::string>> m_ressources;
};