#include "pch.h"
#include "EXP_HotLoad.h"

EXP_HotLoad::EXP_HotLoad() {

}

EXP_HotLoad::~EXP_HotLoad() {
	if (m_moduleHandle != NULL) {
		FreeLibrary(m_moduleHandle);
	}
}

bool EXP_HotLoad::LoadLib(LPCWSTR libFile) {
	m_moduleHandle = LoadLibrary(libFile);
	if (m_moduleHandle == NULL) {
		std::wcerr << L"Cannot load lib " << libFile << std::endl;

		return false;
	}

	return true;
}