#include "EXP_HotLoad.h"

#ifdef _WIN32

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
#else

EXP_HotLoad::EXP_HotLoad() {
    
}

EXP_HotLoad::~EXP_HotLoad()
{
    
}

bool EXP_HotLoad::LoadLib(const char* libFile)
{
    m_moduleHandle = dlopen(libFile, RTLD_NOW);
    if(!m_moduleHandle) {
        std::cout << "ERROR loading lib " << libFile << " System message : " << dlerror() << std::endl;
        return false;
    } else {
        return true;
    }
}



#endif //_WIN32
